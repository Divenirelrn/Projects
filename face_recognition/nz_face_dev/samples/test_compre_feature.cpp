//
// Created by YH-Mac on 2020/9/13.
//
#include "../src/feature_extract.h"
#include "../src/mtcnn.h"
#include "../src/silent_live.h"
#include "face_quality.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "stdio.h"
#include "utils/test_folder_helper.h"
#include <string>

using namespace std;

/**
 * argv[1] model_path
 * argv[2] test_dir
 * argv[3] feature_dir
 * argv[4] result_path
 * */

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir pics_dir feature_dir[1x20000] result.txt"
              << std::endl;
    return 0;
  }

  const std::string model_path = argv[1];
  const std::string MTCNN_BIN_1 = model_path + "/det1.bin";
  const std::string MTCNN_BIN_2 = model_path + "/det2.bin";
  const std::string MTCNN_BIN_3 = model_path + "/det3.bin";
  const std::string MTCNN_PARAM_1 = model_path + "/det1.param";
  const std::string MTCNN_PARAM_2 = model_path + "/det2.param";
  const std::string MTCNN_PARAM_3 = model_path + "/det3.param";
  const std::string Y1MODEL = model_path + "/mfngg.mnn";
  const std::string SILENT_LIVE_27 = model_path + "/sfas_27.mnn";
  const std::string SILENT_LIVE_40 = model_path + "/sfas_40.mnn";

  string test_dir = argv[2];
  string feature_dir = argv[3];
  string result_path = argv[4];
  if (!check_folder(test_dir, false) or !check_folder(feature_dir, false)) {
    cout << "Please enter an existing folder path." << endl;
  }

  // 读取特征
  string suffix = "txt";
  vector<string> feature_name;
  vector<string> feature_path_list = readFileListSuffix(
      feature_dir.c_str(), suffix.c_str(), feature_name, false);
  // cv::Mat featureMatrix;
  if (feature_path_list.size() == 0) {
    std::cout << "error:exit" << std::endl;
    exit(0);
  }
  static int feature_len = 20000;
  // for (int i = 0; i < feature_path_list.size(); ++i) {
  cv::Mat zero = cv::Mat::zeros(1, 512, CV_32F);
  //  vector<float> vec = read_feat_txt(feature_path_list[0]);
  //  vector<float> memory_20000(vec.size() * feature_len);
  //  for(int i = 0  ; i < feature_len ;++i)
  //  {
  //    memcpy(memory_20000.data()+i*512*4 , vec.data() ,512*4);
  //  }
  cv::Mat featureMatrix(20000, 512, CV_32F);
  //  for (int i = 0; i < feature_len; ++i) {
  //    if(i%5000==0)
  //      std::cout<<"i:"<<i<<std::endl;
  //    if (vec.size() == 512) {
  //      cv::Mat feat(vec);
  //      if (i == 0) {
  //        featureMatrix = feat.t();
  //      } else {
  //        cv::vconcat(featureMatrix, feat.t(), featureMatrix);
  //      }
  //    } else {
  //      if (i == 0) {
  //        featureMatrix = zero;
  //      } else {
  //        cv::vconcat(featureMatrix, zero, featureMatrix);
  //      }
  //    }
  //  }
  std::cout << "featureMatrix size rows:" << featureMatrix.rows
            << " cols:" << featureMatrix.cols << std::endl;
  //    cout << featureMatrix << endl;

  // 加载人脸检测模块
  vector<string> mtcnn_bin_list{MTCNN_BIN_1, MTCNN_BIN_2, MTCNN_BIN_3};
  vector<string> mtcnn_param_list{MTCNN_PARAM_1, MTCNN_PARAM_2, MTCNN_PARAM_3};
  MTCNN face_detector(mtcnn_param_list, mtcnn_bin_list);
  // 特提取模块
  FeatureExtract extract(Y1MODEL);
  // 静默活体检测模块
  std::vector<std::string> silent_live_models_path{SILENT_LIVE_27,
                                                   SILENT_LIVE_40};
  SilentLive silentLive(silent_live_models_path);
  // 关键点模块
  // 人脸质量判断模块
  const std::string QUALITY = model_path + "/pose.mnn";
  FaceQuality face_quality(QUALITY);
  // 读取需要测试图片
  string suffix_img = "jpg";
  vector<string> file_names;
  vector<string> path_list = readFileListSuffix(
      test_dir.c_str(), suffix_img.c_str(), file_names, false);

  Timer timer;
  std::ofstream txt(result_path);
  for (int idx = 0; idx < path_list.size(); ++idx) {
    cv::Mat image = cv::imread(path_list[idx]);
    timer.tic();
    // 人脸检测
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
    cv::Rect rect(mBox.x1, mBox.y1, mBox.x2 - mBox.x1, mBox.y2 - mBox.y1);
    // 静默活体
    float silent_result = silentLive.check(image, rect);
    cout << "silent result: " << silent_result << endl;
    // 人脸特征提取
    float dst_arr[10];
    for (int i = 0; i < 5; i += 1) {
      dst_arr[i * 2] = mBox.ppoint[i];
      dst_arr[i * 2 + 1] = mBox.ppoint[i + 5];
    }
    cv::Mat dst(5, 2, CV_32F);
    dst.data = (uchar *)dst_arr;
    cv::Mat transformed;
    extract.Alignment(image, transformed, rect, dst);
    cv::Rect2f mRect(mBox.x1, mBox.y1, mBox.x2 - mBox.x1, mBox.y2 - mBox.y1);
    face_quality.Extract(image, mRect);
    cv::Mat feature;
    extract.Extract(transformed, feature);
    cv::Mat compare_res = featureMatrix * feature;
    // cout << compare_res << endl;

    double diff = timer.toc("process ");
    txt << file_names[idx] << " " << diff << "\n";
    //        cv::imshow("result", image);
    //        cv::waitKey(0);
  }
  txt.close();
  cout << "save the file : " << result_path << endl;

  return 0;
}
