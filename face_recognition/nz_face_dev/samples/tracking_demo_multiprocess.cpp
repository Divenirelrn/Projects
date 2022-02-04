#include <queue>
#include <opencv2/opencv.hpp>
#include <string>
#include <mutex>
#include <thread>
#include "zip_wrapper.h"
#include "face_info.h"
#include "../src/c_api/c_api.h"
#include "../src/c_api/c_api_internal.h"
#include "landmark_tracker.h"
#include "utils/test_helper.h"

std::string model_path="";
std::string video_path="";
using namespace cv;

typedef struct  message_face
{
  unsigned char* best_rgb_buffer;
  unsigned char* best_ir_buffer;
  int            rgb_size;
  int            ir_size;
  int            img_width;      // source image width
  int            img_height;     // source image height
  cv::Rect       best_bbox;
  float          confidence;
  int            face_id;
  int            continous_times;
  double*        trans_matrix;   // matrix, need by huoti alg
}message_face;

queue<message_face*> faceinfo;
mutex faceinfo_lock;

void DrawPointsOnImage(cv::Mat &image, const cv::Rect rect, const std::vector<cv::Point2f> &points) {
  cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 1, 8, 0);
  int shift = 3;
  int factor = (1 << shift);
  for (auto lmk : points) {
    cv::circle(image, cv::Point(lmk.x * factor + 0.5, lmk.y * factor + 0.5), 1, cv::Scalar(255, 255, 0), 2, cv::LINE_AA, shift);
  }
}

void track_fun() {
  cv::VideoCapture video(video_path);
  std::cout<< "inputvideo sucess" <<std::endl;
  cv::Mat image;
  video >> image;
  printf("[track_thread] image rows:%d, cols:%d\n", image.rows, image.cols);
  std::string model_face_tracker  = model_path + "/face_tracker.bin";

  // for tracking
  SG_LandmarkTracker *sgLandmarkTracker           = SG_CreateLandmarkTracker(model_face_tracker.c_str());
  printf("[track_thread] load tracking model sucess\n");

  SG_CameraStream *streaming                      = SG_CreateCameraStream();
  SG_CameraStreamSetRotationMode(streaming, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(streaming, STREAM_YUV_NV21);

  int num = 0;
  set<int> track_set;
  while (!image.empty()) {
    video >> image;
    num += 1;
    printf("-----------------------------------------\n");
    printf("[track_thread] process %d frame\n", num);
    printf("-----------------------------------------\n");

    TestUtils::rotate(image, image, SG::ROTATION_0);
    uint8_t *data    = TestUtils::rgb2nv21(image);
    
    double timeStart = (double)cv::getTickCount();
    SG_CameraStreamSetData(streaming, data, image.cols, image.rows);
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, streaming, false);
    double nTime = ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();

    vector<FaceInfo> faces = sgLandmarkTracker->impl.trackingFace;
    for (auto face : faces) {
      int track_id = face.GetTrackingId();
      if (face.continous_times >= CONST::continus_track_times && track_set.count(track_id) == 0) {
        printf("[track_thread] push face to queue, face id is %d, min_angle:%f\n", track_id, face.min_angle);
        track_set.insert(track_id);
        
        faceinfo_lock.lock();

        message_face* tmp_face = (message_face*)malloc(sizeof(message_face));
        printf("[track_thread] faceinfo sizeof:%d, rgb_size:%d, ir_size:%d\n", sizeof(message_face), face.rgb_size, face.ir_size);
        
        tmp_face->best_rgb_buffer = new unsigned char[face.rgb_size];
        memcpy(tmp_face->best_rgb_buffer, face.best_rgb_buffer, face.rgb_size);
        tmp_face->best_ir_buffer  = new unsigned char[face.ir_size];
        memcpy(tmp_face->best_ir_buffer, face.best_ir_buffer, face.ir_size);

        tmp_face->img_width           = 1280;
        tmp_face->img_height          = 720;
        tmp_face->confidence      = face.confidence_;
        tmp_face->best_bbox       = face.best_bbox;
        tmp_face->face_id         = face.face_id_;
        tmp_face->continous_times = face.continous_times;
        int lence                 = face.trans_matrix_.rows * face.trans_matrix_.cols;
        tmp_face->trans_matrix    = new double[lence];
       
        //for (int i=0; i<2; i++) {
        //  for (int j=0; j<3; j++) {
        //    double tmp = face.trans_matrix_.at<double>(i,j);
        //    printf("%f,", tmp);
        //  }
        //} 
        //printf("\n");
        memcpy(tmp_face->trans_matrix, face.trans_matrix_.data, lence*sizeof(double));
        //for (int i=0; i<6; i++) {
        //  printf("%f,", tmp_face->trans_matrix[i]);
        //}
        printf("\ncv::mat:rows:%d, cols:%d, type:%d\n", face.trans_matrix_.rows, face.trans_matrix_.cols, face.trans_matrix_.type());
        //exit(0);
        //tmp_face->trans_matrix    = face.trans_matrix_.clone();
        printf("[track_thread] tmp face: x:%d, y:%d, w:%d, h:%d, cof:%f\n", tmp_face->best_bbox.x, tmp_face->best_bbox.y, tmp_face->best_bbox.width, tmp_face->best_bbox.height, tmp_face->confidence);
        faceinfo.push(tmp_face); 
        faceinfo_lock.unlock();

        printf("[track_thread] faceinfo size:%d\n", faceinfo.size());
      }
      
      DrawPointsOnImage(image, face.GetRect(), face.GetLanmdark());
      string text       = to_string(face.GetTrackingId());
      string text1      = to_string(num);
      int font_face     = cv::FONT_HERSHEY_COMPLEX;
      double font_scale = 2;
      int thickness     = 2;
      int baseline;
      putText(image, text1, cv::Point(60, 60), font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);
      putText(image, text, cv::Point(face.GetRect().x, face.GetRect().y), font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);
    }
    printf("-----------------------------------------\n");
    std::cout << "[track_thread] " << faces.size()  <<" faces need time: " << nTime * 1000 << "ms"<< std::endl;
    printf("-----------------------------------------\n");

    string image_name ="/sdk/yangjunpei/nazhi_v5/test_images/track_result/"+ to_string(num) + ".jpg";
    cv::imwrite(image_name, image);
  }
  SG_ReleaseLandmarkTracker(sgLandmarkTracker);
}

void recog_fun() {
  // int : track id   bool：if have recog
  // std::map<int, int> reco_map;

   // only for detection
  std::string model_face_tracker                  = model_path + "/face_tracker.bin";
  std::string model_face_industry                 = model_path + "/face_im_e.bin";
  //SG_LandmarkTracker *sgLandmarkTracker_rec       = SG_CreateLandmarkTracker(model_face_tracker.c_str());
  // only for huoti alg eletr picture
  RetinaFacev2Mnn* sgdetection                    = SG_CreateDetector(model_face_tracker.c_str());
  SG_ModelLoader *model_loader                    = SG_CreateModelLoader(model_face_industry.c_str());
  SG_FaceLivnessIR *sgIrLivnessDetector           = SG_CreateFaceLivenessDetectorIR(model_loader);
  //SG_FaceLivnessMonocular *sgFaceLivnessMonocular = SG_CreateFaceLivenessDetectorMonocular(model_loader);
  SG_FaceEmbedding *sgFaceEmbedding               = SG_CreateFaceEmbedder(model_loader);
  SG_CameraStream *streaming_rec                  = SG_CreateCameraStream();
  printf("[rec_thread] load recognition model sucess\n");

  SG_CameraStreamSetRotationMode(streaming_rec, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(streaming_rec, STREAM_YUV_NV21);

  int num = 0;
  while (1) {
    //printf("[reco_thread] faceinfo.size:%d\n", faceinfo.size());
    faceinfo_lock.lock();
    if (!faceinfo.empty()) {
      message_face* cur = faceinfo.front();
      //faceinfo.pop();
      int track_id = cur->face_id;

      num += 1;
      printf("-----------------------------------------\n");
      printf("[reco_thread] process %d face, face_id %d\n", num, track_id);
      printf("-----------------------------------------\n");
      
      SG_FaceResult result;
      SG_FaceResult result_for_huoti;

      // attention this rect is not scaled
      result.rect          = cur->best_bbox;
      result.confidence    = cur->confidence;
      cv::Mat tmp(2,3,6, cur->trans_matrix);
      result.trans_matrix  = tmp.clone();
      result_for_huoti     = result;

      double timeStart = (double)cv::getTickCount();
      SG_CameraStreamSetData(streaming_rec, cur->best_ir_buffer, cur->img_width,cur->img_height);
     
      #if 1 
      // detect if eletronic picture
      BoxInfo boxdet;
      boxdet.x1=result.rect.x;
      boxdet.y1=result.rect.y;
      boxdet.x2=result.rect.x+result.rect.width;
      boxdet.y2=result.rect.y+result.rect.height;

      std::vector<BoxInfo> boxes;
      SG_DetectorIrImage(sgdetection, streaming_rec, boxdet, boxes);
      
      // use ir detect result to check alive
      if (boxes.size() >= 1) {
        std::vector<cv::Point2f> camera_pts;
        float scale = streaming_rec->impl.GetPreviewScale();
        printf("[reco_thread] x1:%f, y1:%f, x2:%f, y2:%f, scale:%f\n", boxes[0].x1, boxes[0].y1, boxes[0].x2, boxes[0].y2, scale);
        
        int cx = (boxes[0].x2 + boxes[0].x1) / 2;
        int cy = (boxes[0].y2 + boxes[0].y1) / 2;
        int R  = std::max(boxes[0].x2-boxes[0].x1, boxes[0].y2-boxes[0].y1) / 2;
        //int R_padding = static_cast<int>(R * (1 + padding_ratio));
        int x1 = cx - R;
        int y1 = cy - R;
        int x2 = cx + R;
        int y2 = cy + R;
        int width = x2 - x1;
        int height = y2 - y1;
        assert(width > 0);
        assert(height > 0);
        assert(height == width);
    // have wrong problem, height==width,0<x1<

    //cv::Rect box_square(x1, y1, width, height);
        camera_pts.push_back(cv::Point2f(x1/scale, y1/scale));
        camera_pts.push_back(cv::Point2f(x2/scale, y1/scale));
        camera_pts.push_back(cv::Point2f(x2/scale, y2/scale));
        printf("[reco_thread] %d,%d,%d,%d, %f,%f,%f,%f\n", x1,y1,x2,y2, x1/scale, y1/scale, x2/scale, y2/scale);
        std::vector<cv::Point2f> dst_pts = {{0, 0}, {112, 0}, {112, 112}};
        assert(dst_pts.size() == camera_pts.size());
        cv::Mat affine = cv::getAffineTransform(camera_pts, dst_pts);
        result_for_huoti.trans_matrix = affine.clone();
      }
      float status = SG_FaceLivenessDetectorIRCheckAlive(sgIrLivnessDetector, streaming_rec, &result_for_huoti);
      printf("[reco_thread] ircheck socre:%f\n", status);
      //exit(0);
      #endif

      //SG_LandmarkTrackerProcessFrame(sgLandmarkTracker_rec, streaming_rec, true);
      double nTime = ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
      
      //for (int i=0; i<2; i++) {
      //    for (int j=0; j<3; j++) {
      //      double tmp = result.trans_matrix.at<double>(i,j);
      //      printf("%f,", tmp);
      //    }
      //  }
      //printf("\n");
      //exit(0);
      //result.trans_matrix  = cur->trans_matrix.clone();
      //cur->trans_matrix_.copyTo(result.trans_matrix);
      
      SG_FaceEmbedderGetFeature(sgFaceEmbedding, streaming_rec, &result);
      int feature_len = 0;
      float *feature = SG_FaceResultGetFeature(&result, &feature_len);
      printf("[reco_thread]cur face: x:%d, y:%d, w:%d, h:%d, cof:%f\n", cur->best_bbox.x, cur->best_bbox.y, cur->best_bbox.width, cur->best_bbox.height, cur->confidence);
      //exit(0);

      printf("[reco_thread] ");
      for (int i=0; i<10; i++) {
        printf("%f, ", feature[i]);
      }
      printf("\n"); 

      if (cur->best_rgb_buffer != nullptr) {
        free(cur->best_rgb_buffer);
        cur->best_rgb_buffer = nullptr;
      }
      if (cur->best_ir_buffer != nullptr) {
        free(cur->best_ir_buffer);
        cur->best_ir_buffer = nullptr;
      }
      if (cur->trans_matrix != nullptr) {
        free(cur->trans_matrix);
        cur->trans_matrix = nullptr;
      }
      printf("[reco_thread] have recognition, free the image data\n");
      faceinfo.pop();
    }
    faceinfo_lock.unlock();
  }
  SG_ReleaseModelLoader(model_loader);
  //SG_ReleaseLandmarkTracker(sgLandmarkTracker_rec);
  SG_ReleaseFaceLivenessDetectorIR(sgIrLivnessDetector);
  //SG_ReleaseFaceLivenessDetectorMonocular(sgFaceLivnessMonocular);
  SG_ReleaseFaceEmbedder(sgFaceEmbedding);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("./tracking_demo path_model path_video\n");
    exit(0);
  }

  model_path = argv[1];
  video_path = argv[2];

  printf("model:%s\n", argv[1]);
  printf("video:%s\n", argv[2]);
  
  //thread track_thread;
  //thread recog_thread;

  thread track_thread(track_fun);
  thread recog_thread(recog_fun);

  track_thread.join();
  recog_thread.join(); 
}
