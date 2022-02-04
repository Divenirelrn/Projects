//
// Created by YH-Mac on 2020/9/13.
//
#include "../src/mtcnn.h"
#include "face_quality.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <string>

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
    std::cout << "models_dir pics_dir result_path" << std::endl;
    return 1;
  }

  const std::string model_path = argv[1];
  const std::string MTCNN_BIN_1 = model_path + "/det1.bin";
  const std::string MTCNN_BIN_2 = model_path + "/det2.bin";
  const std::string MTCNN_BIN_3 = model_path + "/det3.bin";
  const std::string MTCNN_PARAM_1 = model_path + "/det1.param";
  const std::string MTCNN_PARAM_2 = model_path + "/det2.param";
  const std::string MTCNN_PARAM_3 = model_path + "/det3.param";
  const std::string QUALITY = model_path + "/pose.mnn";

  string pics_dir = argv[2];
  string result_path = argv[3];
  if (!check_folder(pics_dir, false)) {
    cout << "Please enter an existing folder path." << endl;
  }
  vector<string> mtcnn_bin_list{MTCNN_BIN_1, MTCNN_BIN_2, MTCNN_BIN_3};
  vector<string> mtcnn_param_list{MTCNN_PARAM_1, MTCNN_PARAM_2, MTCNN_PARAM_3};
  MTCNN face_detector(mtcnn_param_list, mtcnn_bin_list);
  FaceQuality face_quality(QUALITY);

  string suffix_img = "jpg";
  vector<string> file_names;
  vector<string> path_list = readFileListSuffix(
      pics_dir.c_str(), suffix_img.c_str(), file_names, false);

  Timer timer;
  std::ofstream txt(result_path);
  for (int idx = 0; idx < path_list.size(); ++idx) {
    cv::Mat image = cv::imread(path_list[idx]);
    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(
        image.data, ncnn::Mat::PIXEL_BGR2RGB, image.cols, image.rows);
    std::vector<Bbox> finalBbox;
    face_detector.detectMaxFace(ncnn_img, finalBbox);
    const int num_box = finalBbox.size();
    if (num_box == 0) {
      cout << path_list[idx] << ", No face detected." << endl;
      continue;
    }
    Bbox mBox = finalBbox[0];
    cv::Rect2f mRect(mBox.x1, mBox.y1, mBox.x2 - mBox.x1, mBox.y2 - mBox.y1);
    timer.tic();
    face_quality.Extract(image, mRect);
    double diff = timer.toc("process ");
    float yaw = face_quality.getYaw();
    float roll = face_quality.getRoll();
    float pitch = face_quality.getPitch();
    std::vector<float> blurs = face_quality.getQuality();
    printf("yaw:%f roll:%f pitch:%f\n", yaw, roll, pitch);
    printf("blur: ");
    for (auto &blur : blurs) {
      printf("%f ", blur);
    }
    printf("\n");
    txt << file_names[idx] << " "
        << " " << diff << "\n";
  }
  txt.close();
  cout << "save the file : " << result_path << endl;

  return 0;
}
