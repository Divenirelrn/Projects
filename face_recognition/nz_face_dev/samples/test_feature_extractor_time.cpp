//
// Created by YH-Mac on 2020/9/13.
//
#include "../src/feature_extract.h"
#include "../src/mtcnn.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <string>
//#include "../src/feature_extrator.h"

using namespace std;

//#define MTCNN_BIN_1 "models/det1.bin"
//#define MTCNN_BIN_2 "models/det2.bin"
//#define MTCNN_BIN_3 "models/det3.bin"
//#define MTCNN_PARAM_1 "models/det1.param"
//#define MTCNN_PARAM_2 "models/det2.param"
//#define MTCNN_PARAM_3 "models/det3.param"
//#define Y1MODEL "models/mfngg.mnn"
#define LOOP 500

/**
 * argv[1] model_path
 * argv[2] image_path
 * argv[3] result_path
 * */

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir filename result_path time" << std::endl;
    return 0;
  }

  const std::string model_path = argv[1];
  const string image_path = argv[2];
  const string result_path = argv[3];

  const std::string MTCNN_BIN_1 = model_path + "/det1.bin";
  const std::string MTCNN_BIN_2 = model_path + "/det2.bin";
  const std::string MTCNN_BIN_3 = model_path + "/det3.bin";
  const std::string MTCNN_PARAM_1 = model_path + "/det1.param";
  const std::string MTCNN_PARAM_2 = model_path + "/det2.param";
  const std::string MTCNN_PARAM_3 = model_path + "/det3.param";
  const std::string Y1MODEL = model_path + "/mfngg.mnn";
  const std::string SILENT_LIVE_27 = model_path + "/sfas_27.mnn";
  const std::string SILENT_LIVE_40 = model_path + "/sfas_40.mnn";

  if (!check_folder(image_path, false)) {
    cout << "Please enter an existing image path." << endl;
  }
  vector<string> mtcnn_bin_list{MTCNN_BIN_1, MTCNN_BIN_2, MTCNN_BIN_3};
  vector<string> mtcnn_param_list{MTCNN_PARAM_1, MTCNN_PARAM_2, MTCNN_PARAM_3};
  MTCNN face_detector(mtcnn_param_list, mtcnn_bin_list);
  FeatureExtract extract(Y1MODEL);
  Timer timer;
  std::ofstream txt(result_path);
  for (int i = 0; i < LOOP; ++i) {
    cv::Mat image = cv::imread(image_path);
    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(
        image.data, ncnn::Mat::PIXEL_BGR2RGB, image.cols, image.rows);
    std::vector<Bbox> finalBbox;
    face_detector.detectMaxFace(ncnn_img, finalBbox);
    Bbox mBox = finalBbox[0];
    cv::Rect rect(mBox.x1, mBox.y1, mBox.x2 - mBox.x1, mBox.y2 - mBox.y1);
    float dst_arr[10];
    for (int i = 0; i < 5; i += 1) {
      dst_arr[i * 2] = mBox.ppoint[i];
      dst_arr[i * 2 + 1] = mBox.ppoint[i + 5];
    }
    timer.tic();
    cv::Mat dst(5, 2, CV_32F);
    dst.data = (uchar *)dst_arr;
    cv::Mat transformed;
    extract.Alignment(image, transformed, rect, dst);
    vector<float> feature = extract.Extract(transformed);
    double diff = timer.toc("loop");
    txt << dePrefix(image_path, "/") << " " << diff << "\n";
  }
  txt.close();
  cout << "save the file : " << result_path << endl;

  return 0;
}
