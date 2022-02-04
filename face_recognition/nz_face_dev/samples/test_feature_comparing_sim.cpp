//
// Created by YH-Mac on 2020/9/13.
//

#include "../src/mtcnn.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <string>

using namespace std;

/**
 * argv[1] model_path
 * argv[2] test_folder_path
 * argv[3] base_folder_path
 * argv[4] result_path
 * */

int main(int argc, char **argv) {
  if (argc != 5) {

    std::cout << argv[0] << " ";
    std::cout << "models_dir test_pics_dir features_dir result_path"
              << std::endl;
    return 0;
  }
  string model_path = argv[1];
  string test_folder_path = argv[2];
  string base_folder_path = argv[3];
  string result_path = argv[4];
  if (!check_folder(test_folder_path, false) or
      !check_folder(base_folder_path, false)) {
    cout << "Please enter an existing folder path." << endl;
  }
  string suffix = "txt";
  vector<string> test_file_names;
  vector<string> test_path_list = readFileListSuffix(
      test_folder_path.c_str(), suffix.c_str(), test_file_names, false);

  vector<string> base_file_names;
  vector<string> base_path_list = readFileListSuffix(
      base_folder_path.c_str(), suffix.c_str(), base_file_names, false);
  // base feature matrix
  cv::Mat featureMatrix;
  for (int i = 0; i < base_path_list.size(); ++i) {
    vector<float> vec = read_feat_txt(base_path_list[i]);
    if (vec.size() == 512) {
      cv::Mat feat(vec);
      if (i == 0) {
        featureMatrix = feat.t();
      } else {
        cv::vconcat(featureMatrix, feat.t(), featureMatrix);
      }
    } else {
      cv::Mat zero = cv::Mat::zeros(1, 512, CV_32F);
      if (i == 0) {
        featureMatrix = zero;
      } else {
        cv::vconcat(featureMatrix, zero, featureMatrix);
      }
    }
  }

  // test features list
  vector<cv::Mat> test_features;
  for (int j = 0; j < test_path_list.size(); ++j) {
    vector<float> vec = read_feat_txt(test_path_list[j]);
    if (vec.size() == 512) {
      cv::Mat feat(vec);
      test_features.push_back(feat.clone());
    } else {
      cv::Mat zero = cv::Mat::zeros(512, 1, CV_32F);
      test_features.push_back(zero);
    }
  }

  //    cout << test_features[1] << endl;

  // start comparing
  std::ofstream txt(result_path);
  for (int k = 0; k < test_features.size(); ++k) {
    cv::Mat res = featureMatrix * test_features[k];
    int max_idx = -1;
    int *max_idx_p = &max_idx;
    //        cout << res << endl;
    cv::minMaxIdx(res, NULL, NULL, NULL, max_idx_p);
    //        cout  << max_idx  << ": " << res.at<float>(max_idx, 0) <<  endl;
    txt << deSuffix(test_file_names[k], ".") << " "
        << deSuffix(base_file_names[max_idx], ".") << " "
        << res.at<float>(0, max_idx) << "\n";
  }
  cout << "save the file : " << result_path << endl;

  return 0;
}