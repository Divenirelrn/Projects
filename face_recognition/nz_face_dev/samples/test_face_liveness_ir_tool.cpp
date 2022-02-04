//
// Created by YH-Mac on 2020/9/13.
//
//#include "../src/mtcnn.h"
//#include "../src/silent_live_ir.h"
//#include "model_loader.h"
#include "../src/c_api/c_api.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <string>

//#define MTCNN_BIN_1 "models/det1.bin"
//#define MTCNN_BIN_2 "models/det2.bin"
//#define MTCNN_BIN_3 "models/det3.bin"
//#define MTCNN_PARAM_1 "models/det1.param"
//#define MTCNN_PARAM_2 "models/det2.param"
//#define MTCNN_PARAM_3 "models/det3.param"
//#define SILENT_LIVE_27 "models/sfas_27.mnn"
//#define SILENT_LIVE_40 "models/sfas_40.mnn"

#define SILENT_THRESHOLD 0.92
using namespace std;

/**
 * argv[1] model_path
 * argv[2] pics_dir
 * argv[3] result_path
 * */
int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir pics_ir_dir result_path" << std::endl;
    return 1;
  }

  string ir_dir = argv[2];
  string result_path = argv[3];
  if (!check_folder(ir_dir, false)) {
    cout << "Please enter an existing folder path." << endl;
  }
  // 加载人脸检测模块

  string model_dir = argv[1];
  std::string model_face_industry = model_dir + "/face_im_e.bin";
  std::string model_face_tracker = model_dir + "/face_tracker.bin";
  std::cout << "face_in:" << model_face_industry;
  std::cout << "face_tracker:" << model_face_tracker;

  SG_ModelLoader *model_loader =
      SG_CreateModelLoader(model_face_industry.c_str());
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_face_tracker.c_str());
  SG_FaceLivnessIR *sgIrLivnessDetector =
      SG_CreateFaceLivenessDetectorIR(model_loader);
  SG_FaceLivnessMonocular *sgFaceLivnessMonocular =
      SG_CreateFaceLivenessDetectorMonocular(model_loader);
  SG_FaceEmbedding *sgFaceEmbedding = SG_CreateFaceEmbedder(model_loader);

  // cv::Mat image =
  // cv::imread("/Users/yujinke/Downloads/skgg/data/padData/3_liveness/3_liveness2_pad/rgb/00037.jpg");

  // 读取需要测试图片
  string suffix_img = "jpg";
  vector<string> file_names;
  vector<string> path_list =
      readFileListSuffix(ir_dir.c_str(), suffix_img.c_str(), file_names, false);

  Timer timer;
  std::cout << "result_path:" << result_path << std::endl;
  std::ofstream txt(result_path);
  for (int idx = 0; idx < path_list.size(); ++idx) {
    std::cout << "read image" << path_list[idx] << std::endl;
    cv::Mat nir_image = cv::imread(path_list[idx]);
    SG_CameraStream *IR_image = SG_CreateCameraStream();
    SG_CameraStreamSetRotationMode(IR_image, CAMERA_ROTATION_0);
    SG_CameraStreamSetStreamFormat(IR_image, STREAM_BGR);
    SG_CameraStreamSetData(IR_image, nir_image.data, nir_image.cols,
                           nir_image.rows);
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, IR_image, true);
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    if (face_num == 1) {
      SG_FaceResult *result =
          SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, 0);
      float status = SG_FaceLivenessDetectorIRCheckAlive(sgIrLivnessDetector,
                                                         IR_image, result);
      timer.tic();
      double diff = timer.toc("process ");
      // int out = status ;
      txt << "rgb/" << file_names[idx] << " nir/" << file_names[idx] << " "
          << status << " " << diff << "\n";
      SG_ReleaseFaceResult(result);
    } else {
      timer.tic();
      double diff = timer.toc("process ");

      txt << "rgb/" << file_names[idx] << " nir/" << file_names[idx] << " "
          << -1 << " " << diff << "\n";
    }
  }
  txt.close();
  cout << "save the file : " << result_path << endl;

  return 0;
}
