//
// Created by YH-Mac on 2020/9/11.
//
#include "c_api/c_api.h"
#include "c_api/c_api_internal.h"
#include "face_quality.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <sys/time.h>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>

//#define MTCNN_BIN_1 "models/det1.bin"
//#define MTCNN_BIN_2 "models/det2.bin"
//#define MTCNN_BIN_3 "models/det3.bin"
//#define MTCNN_PARAM_1 "models/det1.param"
//#define MTCNN_PARAM_2 "models/det2.param"
//#define MTCNN_PARAM_3 "models/det3.param"
//#define Y1MODEL "models/mfngg.mnn"

using namespace std;

/**
 * argv[1] model_path
 * argv[2] images_folder_path
 * argv[3] features_folder_path
 * */

static double get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}

void print_feature(const std::string &filename , std::vector<float> &feature){
    int feature_len = feature.size();
    std::cout<<filename<<":";
    for(int i =0 ; i < feature_len ; i++)
    {
        std::cout<<feature[i] << ",";
    }
    std::cout<<std::endl;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir pics_dir" << std::endl;
    return 0;
  }

  string images_folder_path = argv[2];
  // string features_folder_path = argv[3];

  string model_dir = argv[1];
  std::string model_face_industry = model_dir + "/face_im_e.bin";
  std::string model_face_tracker = model_dir + "/face_tracker.bin";
  double avg_time = 0.0;
  int detect_num = 0;
  ofstream in;
  in.open("/sdk/yangjunpei/nazhi_v5/zhangwenjing/face_quality/error_norm_nazhi_2_1/nazhi-12-21-cpu_error_norm_nazhi_2_1_quality_pred_cpp.txt",ios::trunc); //ios::trunc表示在

  SG_ModelLoader *model_loader =
      SG_CreateModelLoader(model_face_industry.c_str());
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_face_tracker.c_str());
  // SG_FaceLivnessIR *sgIrLivnessDetector =
  //     SG_CreateFaceLivenessDetectorIR(model_loader);
  // SG_FaceLivnessMonocular *sgFaceLivnessMonocular =
  //     SG_CreateFaceLivenessDetectorMonocular(model_loader);
  SG_FaceEmbedding *sgFaceEmbedding = SG_CreateFaceEmbedder(model_loader);

  if (check_folder(images_folder_path, false)) {
    std::cout << "images_folder_path: " << images_folder_path << std::endl;
  } else {
    std::cout << "images_folder_path folder does not exist." << std::endl;
    return 1;
  }
  // check_folder(features_folder_path, true);
  // Read folder
  string suffix = "jpg";
  vector<string> file_names;
  //string strformat[] = {"jpg", "png", "bmp", "jpeg", "JPG", "PNG", "BMP", "JPEG"};
  //vector<string> suffix(strformat, strformat+8);

  vector<string> path_list = readFileListSuffix(
      images_folder_path.c_str(), suffix.c_str(), file_names, false);
  std::cout << "len path list" << path_list.size() << std::endl;
  // exit(0);
  int show_cout = 3;
  int show_front_index =
      path_list.size() < show_cout ? path_list.size() : show_cout;
  for (int i = 0; i < show_front_index; ++i) {
    cout << path_list[i] << endl;
  }
  cout << "....." << endl;
  cout << "Search for ." + suffix + " file: " << path_list.size() << endl;
  // load MTCNN
  Timer timer;
  std::string txt = "";
  std::vector<float> quality_all;
  std::vector<cv::Point2f> pts5_all;
  float yaw_all;
  float roll_all;
  float pitch_all;
  for (int idx = 0; idx < path_list.size(); idx++) {
    // SG_FaceEmbedderSetTag(sgFaceEmbedding, file_names[idx].c_str());
    std::string path_list_idx = path_list[idx];
    cv::Mat image = cv::imread(path_list[idx]);
    std::cout << "filename:" << file_names[idx] << std::endl;
    std::cout << "path_list:" << path_list[idx] << std::endl;
    SG_CameraStream *IR_image = SG_CreateCameraStream();
    SG_CameraStreamSetRotationMode(IR_image, CAMERA_ROTATION_0);
    SG_CameraStreamSetStreamFormat(IR_image, STREAM_BGR);
    SG_CameraStreamSetData(IR_image, image.data, image.cols, image.rows);
    timer.tic();
    // double start_time = get_current_time();
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, IR_image, true);
    // double end_time = get_current_time();
    // double detect = timer.toc("detect ");
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    std::cout << "face num " << face_num << std::endl;
    if (face_num > 0) {
      timer.tic();

      //face quality
      detect_num += 1;
      float pv_scale = IR_image->impl.GetPreviewScale();
      // for (int tmp_num = 0; tmp_num < face_num; tmp_num++){
      SG_FaceResult *result =
          SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, 0);
      cv::Rect2f detect_box_ = result->rect;
      std::cout<<"CONST::detection_scale:"<<CONST::detection_scale<<std::endl;
      std::cout<<"result->rect" << result->rect.x<< " " <<result->rect.y << " " << result->rect.width << " "<<result->rect.height<<std::endl;
      detect_box_.x /= pv_scale;
      detect_box_.y /= pv_scale;
      detect_box_.width /= pv_scale;
      detect_box_.height /= pv_scale;
      double start_time = get_current_time();
      cv::Mat affine_crop = sgFaceEmbedding->impl_quality.ComputeCropMatrix(detect_box_);
      //cv::imwrite("affine_crop.png" , affine_crop);
      cv::Mat affine_crop_inv;
      cv::invertAffineTransform(affine_crop, affine_crop_inv);
      affine_crop.convertTo(affine_crop, CV_64F);
      affine_crop_inv.convertTo(affine_crop_inv, CV_64F);
      cv::Mat crop_pre_infer = IR_image->impl.GetAffineRGBImage(affine_crop, 96, 96);
      //cv::imwrite("crop_pre_infer.png" , crop_pre_infer);

      sgFaceEmbedding->impl_quality.Extract(crop_pre_infer);
      double end_time = get_current_time();
      double quality_time = (end_time - start_time);
      avg_time += quality_time;
      in<<file_names[idx]<<","<<quality_time;
      quality_all = sgFaceEmbedding->impl_quality.getQuality();
      pts5_all = sgFaceEmbedding->impl_quality.getPoints();
      yaw_all = sgFaceEmbedding->impl_quality.getYaw();
      roll_all = sgFaceEmbedding->impl_quality.getRoll();
      pitch_all = sgFaceEmbedding->impl_quality.getPitch();
      std::vector<cv::Point2f> pts5_align = ApplyTransformToPoints(pts5_all, affine_crop_inv);
      SG_ReleaseFaceResult(result);
      std::stringstream pts;
      for (auto &p : pts5_all) {
        pts << p.x << " " << p.y << " ";
      }
      std::stringstream quality_blur;
      for (int i=0; i<5; i++) {
        quality_blur << quality_all[i] << " ";
      }
      std::cout << "pts5_all:" << pts.str() << std::endl;
      std::cout << "quality all:" << quality_blur.str() << std::endl;
      // std::cout << "pts5 all:" << pts5_all[0] << std::endl;
      std::cout << "yaw all:" << yaw_all << std::endl;
      std::cout << "roll all:" << roll_all << std::endl;
      std::cout << "pitch all:" << pitch_all << std::endl;
      // if(abs(yaw_all)<30 and abs(roll_all)<30 and abs(pitch_all)<30){
      //   in<<","<<0;
      // }
      // else{
      //   in<<","<<1;
      // }
      
      in<<","<<yaw_all<<","<<pitch_all<<","<<roll_all;

      float blur1 = (quality_all[0]+quality_all[1])/2;
      float blur2 = quality_all[2];
      float blur3 = (quality_all[3]+quality_all[4])/2;
      // if(quality_all[0]<0.5 and quality_all[1]<0.5 and quality_all[2]<0.5 and quality_all[3]<0.5 and quality_all[4]<0.5){
      // if(blur1<0.2 and blur2<0.2 and blur3<0.2){
      //   in<<","<<0;
      // }
      // else{
      //   in<<","<<1;
      // }
      in<<","<<blur1<<","<<blur2<<","<<blur3;
      in<<std::endl;
    }else {
       std::cout<< file_names[idx] <<"***this image find no face***" <<std::endl;
    }


      // //face detection
      // double detect_time = (end_time - start_time);
      // avg_time += detect_time;
      // detect_num += 1;
      // in<<file_names[idx]<<","<<(int)face_num<<","<<detect_time;
      // float pv_scale = IR_image->impl.GetPreviewScale();
      // for (int tmp_num = 0; tmp_num < face_num; tmp_num++){
      //   SG_FaceResult *result =
      //       SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, tmp_num);
      //   cv::Rect2f detect_box_ = result->rect;
      //   detect_box_.x /= pv_scale;
      //   detect_box_.y /= pv_scale;
      //   detect_box_.width /= pv_scale;
      //   detect_box_.height /= pv_scale;
      //   in<<","<<(int)(detect_box_.x)<<","<<(int)(detect_box_.y)<<","<<(int)(detect_box_.x+detect_box_.width)<<","<<(int)(detect_box_.y+detect_box_.height);
      //   std::cout << "confidence:" << result->confidence << std::endl;
      //   std::cout << "rect box:" << detect_box_.x<< ' ' << detect_box_.y << ' ' << detect_box_.width << ' ' << detect_box_.height << std::endl;
      //   // std::cout << "landmarks:" << result->landmarks << std::endl;
      //   // SG_FaceEmbedderGetFeature(sgFaceEmbedding, IR_image, result);
      //   double diff = timer.toc("process ");
      //   std::cout << "test!!!" << std::endl;
      //   SG_ReleaseFaceResult(result);
      // }
      
      // //face angle
      // float *euler_angle = new float[3];
      // SG_LandmarkTrackerGetInfoEulerAngle(sgLandmarkTracker, 0, euler_angle, 3);
      // std::cout << "euler_angle:" << euler_angle[0] << ' ' << euler_angle[1] << ' ' << euler_angle[2] << std::endl;
      // int feature_len = 0;
      // float *feature = SG_FaceResultGetFeature(result, &feature_len);
      // std::cout<<"feature_len:"<<feature_len<<std::endl;
      // std::vector<float> feature_v(feature, feature + feature_len);
      // string feat_txt_path =
      //     features_folder_path + "/" + file_names[idx] + ".txt";
      // std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
      // print_feature(path_list_idx, feature_v);
      // extract_feat_to_txt(feat_txt_path, feature_v);

      // //face keypoints
      // float *landmark106 = new float[212];
      // SG_LandmarkTrackerGetInfoLandmarks(sgLandmarkTracker, 0, landmark106);
      // std::cout << "landmark:" << landmark106[0] << landmark106[1] << std::endl;
      
    // } else {
    //    std::cout<<"this image find no face" <<std::endl;
    // }
  }
  in.close();
  avg_time /= detect_num;
  cout << "avg time is : " << avg_time << std::endl;
}
    

// #if 0 
//     else {
//       cv::Mat image_tmp;
//       cv::resize(image, image_tmp, cv::Size(96, 96));
//       sgFaceEmbedding->impl_quality.Extract(image_tmp);
//       std::vector<cv::Point2f> pts_5 =
//           sgFaceEmbedding->impl_quality.getPoints();
//       for (auto &p : pts_5) {
//         p.x *= 112 / 96.0f;
//         p.y *= 112 / 96.0f;
//       }
//       cv::Mat affine = sgFaceEmbedding->impl.GetAlignmentMatrix(pts_5);
//       cv::Mat aligned_112;
//       cv::warpAffine(image, aligned_112, affine, cv::Size(112, 112));

//       int feature_len = 0;
//       SG_FaceResult result;
//       std::vector<float> feature_v = sgFaceEmbedding->impl.Extract(aligned_112);
//       string feat_txt_path =
//           features_folder_path + "/" + file_names[idx] + ".txt";
//       std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
//       extract_feat_to_txt(feat_txt_path, feature_v);
//       print_feature(path_list_idx, feature_v);
//       std::stringstream pts;
//       for (auto &p : pts_5) {
//         pts << p.x << " " << p.y << " ";
//       }

//       cv::imwrite("tmp/" + path_list[idx] + ".png", aligned_112);
//       std::cout << "bbox:" << path_list[idx] << " " << 0 << " " << 0 << " "
//                 << 112 << " " << 112 << " " << pts.str() << std::endl;
//     }
// #endif

//     //        cout << feature << endl;
//     //        cv::imshow("transformed", transformed);
//     //        cv::waitKey(0);
//   }
//   cout << "Feature extraction successful：" << path_list.size() << endl;
//   return 0;
// }
