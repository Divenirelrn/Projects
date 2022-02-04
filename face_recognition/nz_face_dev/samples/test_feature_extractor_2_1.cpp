//
// Created by YH-Mac on 2020/9/11.
//
#include "c_api/c_api.h"
#include "c_api/c_api_internal.h"
#include "face_quality.h"
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
 * argv[3] features_folder_path
 * */

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
  if (argc != 4) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir pics_dir features_dir" << std::endl;
    return 0;
  }

  ofstream in;
  in.open("/sdk/yangjunpei/nazhi_v5/zhangwenjing/face_quality/dogs/nazhi-12-21-arm_face_quality_dogs_cpp_score.txt",ios::trunc);

  string images_folder_path = argv[2];
  string features_folder_path = argv[3];

  string model_dir = argv[1];
  std::string model_face_industry = model_dir + "/face_im_e.bin";
  std::string model_face_tracker = model_dir + "/face_tracker.bin";

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
  check_folder(features_folder_path, true);
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
  for (int idx = 0; idx < path_list.size(); idx++) {
    SG_FaceEmbedderSetTag(sgFaceEmbedding, file_names[idx].c_str());
    std::string path_list_idx = path_list[idx];
    cv::Mat image = cv::imread(path_list[idx]);
    std::cout << "filename:" << file_names[idx] << std::endl;
    std::cout << "path_list:" << path_list[idx] << std::endl;
    SG_CameraStream *IR_image = SG_CreateCameraStream();
    SG_CameraStreamSetRotationMode(IR_image, CAMERA_ROTATION_0);
    SG_CameraStreamSetStreamFormat(IR_image, STREAM_BGR);
    SG_CameraStreamSetData(IR_image, image.data, image.cols, image.rows);
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, IR_image, true);
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    std::cout << "face num " << face_num << std::endl;
    in<<file_names[idx]<<","<<(int)face_num;
    if (face_num > 0) {
      // timer.tic();
      for (int tmp_num = 0; tmp_num < face_num; tmp_num++){
        SG_FaceResult *result =
            SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, tmp_num);
        SG_FaceEmbedderGetFeature(sgFaceEmbedding, IR_image, result);
        // double diff = timer.toc("process ");
        int feature_len = 0;
        float *feature = SG_FaceResultGetFeature(result, &feature_len);
        std::cout<<"feature_len:"<<feature_len<<std::endl;
        std::vector<float> feature_v(feature, feature + feature_len);
        string feat_txt_path =
            features_folder_path + "/" + file_names[idx] + ".txt";
        std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
        // print_feature(path_list_idx, feature_v);
        extract_feat_to_txt(feat_txt_path, feature_v);
        float l2_norm  = sgFaceEmbedding->impl.GetL2Norm();
        in<<","<<l2_norm;
        // float res=0;
        // for (size_t i=0; i<feature_v.size(); i++)
        // {
        //     res += feature_v[i]*feature_v[i];
        // }
        // double r = sqrt(res);
        std::cout << "feature norm:" << l2_norm << std::endl;
        SG_ReleaseFaceResult(result);
      }
      in<<std::endl;
     } else {
       std::cout<<"this image find no face" <<std::endl;
     }
  }
  in.close();
  cout << "Feature extraction successful：" << path_list.size() << endl;
  return 0;
}
