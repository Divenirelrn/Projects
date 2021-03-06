//
// Created by YH-Mac on 2020/9/11.
//
#include "../src/c_api/c_api.h"
#include "../src/c_api/c_api_internal.h"
#include "../src/face_quality.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <sstream>
#include <string>

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
 * argv[3] features_result_file
 * */

void print_feature(const std::string &filename, std::vector<float> &feature) {
  int feature_len = feature.size();
  std::cout << filename << ":";
  for (int i = 0; i < feature_len; i++) {
    std::cout << feature[i] << ",";
  }
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir pics_dir features_result_file [model]" << std::endl;
    return 0;
  }

  string images_folder_path = argv[2];
  string features_folder_path = argv[3];
  // create feature file
  std::ofstream feat_txt(features_folder_path); 

  string model_dir = argv[1];
  std::string model_face_tracker = model_dir + "/face_tracker.bin";
  std::string model_face_industry = "";
  if (argc > 4) {
    model_face_industry = argv[4];
  } else {
    model_face_industry = model_dir + "/face_im_e.bin";
  }

  SG_ModelLoader *model_loader =
      SG_CreateModelLoader(model_face_industry.c_str());
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_face_tracker.c_str());
  SG_FaceLivnessIR *sgIrLivnessDetector =
      SG_CreateFaceLivenessDetectorIR(model_loader);
  SG_FaceLivnessMonocular *sgFaceLivnessMonocular =
      SG_CreateFaceLivenessDetectorMonocular(model_loader);
  SG_FaceEmbedding *sgFaceEmbedding = SG_CreateFaceEmbedder(model_loader);

  if (check_folder(images_folder_path, false)) {
    std::cout << "images_folder_path: " << images_folder_path << std::endl;
  } else {
    std::cout << "images_folder_path folder does not exist." << std::endl;
    return 1;
  }
  //check_folder(features_folder_path, true);
  // Read folder
  //string suffix = "jpg";

  string strformat[] = {"jpg", "png", "bmp", "jpeg", "JPG", "PNG", "BMP", "JPEG"};
  vector<string> suffix(strformat, strformat+8);
  
  for (int i=0; i < suffix.size(); i++) {
      cout << suffix[i] <<endl;
  }  

  vector<string> file_names;
  vector<string> path_list = readFileListSuffix_jpy(
      images_folder_path.c_str(), suffix, file_names, false);
  std::cout << "len path list" << path_list.size() << std::endl;
  // exit(0);
  int show_cout = 3;
  int show_front_index =
      path_list.size() < show_cout ? path_list.size() : show_cout;
  for (int i = 0; i < show_front_index; ++i) {
    cout << path_list[i] << endl;
  }
  cout << "....." << endl;
  cout << "Search for image file: " << path_list.size() << endl;
  // load MTCNN
  Timer timer;
  std::string txt = "";
  
  double det_time = 0;
  double all_det_time = 0;
  int real_det_num = 0;
  double extract_time = 0;
  double all_extract_time = 0;
  //int real_rec_num = 0;

  for (int idx = 0; idx < path_list.size(); idx++) {
    
    //timer.tic();
    SG_FaceEmbedderSetTag(sgFaceEmbedding, file_names[idx].c_str());
    std::string path_list_idx = path_list[idx];
    cv::Mat image = cv::imread(path_list[idx]);
    std::cout << "filename:" << file_names[idx] << std::endl;
    std::cout << "path_list:" << path_list[idx] << std::endl;
    SG_CameraStream *IR_image = SG_CreateCameraStream();
    SG_CameraStreamSetRotationMode(IR_image, CAMERA_ROTATION_0);
    SG_CameraStreamSetStreamFormat(IR_image, STREAM_BGR);
    SG_CameraStreamSetData(IR_image, image.data, image.cols, image.rows);
    timer.tic();
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, IR_image, true);
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    std::cout << "face num " << face_num << std::endl;
    if (face_num > 0) {
      double det_time = timer.toc("process detect");
      real_det_num += 1;
      all_det_time += det_time;
      
      timer.tic();
      SG_FaceResult *result =
          SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, 0);
      SG_FaceEmbedderGetFeature(sgFaceEmbedding, IR_image, result);
      extract_time = timer.toc("process extract");
      all_extract_time += extract_time;      

      int feature_len = 0;
      float *feature = SG_FaceResultGetFeature(result, &feature_len);
      std::cout << "feature_len:" << feature_len << std::endl;
      std::vector<float> feature_v(feature, feature + feature_len);
      //string feat_txt_path =
      //    features_folder_path + "/" + file_names[idx] + ".txt";
      //std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
      //print_feature(path_list_idx, feature_v);
      //extract_feat_to_txt(feat_txt_path, feature_v);
      
       feat_txt << file_names[idx] << ",";
       for (int i = 0; i <feature_v.size(); i++){
           feat_txt << feature_v[i] << "f,";
       }
       feat_txt << det_time << "," << extract_time <<"," << endl;
       //feat_txt << endl;
       SG_ReleaseFaceResult(result);
      cout<<"average det time:" << (all_det_time+0.0001)/real_det_num << "ms " << "average extract time:" << (all_extract_time+0.0001)/real_det_num << "ms" <<endl;
    }else {
      feat_txt << file_names[idx] << ",";
      feat_txt <<"none" <<endl;
    }
    
    #if 0 
    else {
      cv::Mat image_tmp;
      cv::resize(image, image_tmp, cv::Size(96, 96));
      sgFaceEmbedding->impl_quality.Extract(image_tmp);
      std::vector<cv::Point2f> pts_5 =
          sgFaceEmbedding->impl_quality.getPoints();
      for (auto &p : pts_5) {
        p.x *= 112 / 96.0f;
        p.y *= 112 / 96.0f;
      }
      cv::Mat affine = sgFaceEmbedding->impl.GetAlignmentMatrix(pts_5);
      cv::Mat aligned_112;
      cv::warpAffine(image, aligned_112, affine, cv::Size(112, 112));

      int feature_len = 0;
      SG_FaceResult result;
      std::vector<float> feature_v = sgFaceEmbedding->impl.Extract(aligned_112);
      string feat_txt_path =
          features_folder_path + "/" + file_names[idx] + ".txt";
      std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
      extract_feat_to_txt(feat_txt_path, feature_v);
      print_feature(path_list_idx, feature_v);
      std::stringstream pts;
      for (auto &p : pts_5) {
        pts << p.x << " " << p.y << " ";
      }

      cv::imwrite("tmp/" + path_list[idx] + ".png", aligned_112);
      std::cout << "bbox:" << path_list[idx] << " " << 0 << " " << 0 << " "
                << 112 << " " << 112 << " " << pts.str() << std::endl;
    }
    #endif

    //        cout << feature << endl;
    //        cv::imshow("transformed", transformed);
    //        cv::waitKey(0);
  }
  feat_txt.close();
  //cout << "Feature extraction successful???" << path_list.size() << endl;
  return 0;
}
