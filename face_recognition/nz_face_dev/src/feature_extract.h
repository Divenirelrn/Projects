//
// Created by YH-Mac on 2020/9/10.
//

#ifndef LMKTRACKING_LIB_FEATURE_EXTRACT_H
#define LMKTRACKING_LIB_FEATURE_EXTRACT_H
#include "camera_stream.h"
#include "face_preprocess.h"
#include "log.h"
#include "model_infer/mnn_infer.h"
#include <opencv2/opencv.hpp>

using namespace std;
class FeatureExtract {
public:
  FeatureExtract();
  /**
   * 初始化
   * @param model_path
   * */
  FeatureExtract(string model_path);
  void Reset(string model_path);
  void Reset(const pr::ModelLoader &model);
  vector<float> Extract(cv::Mat &images);
  void Extract(cv::Mat &images, cv::Mat &feature);
  void Alignment(cv::Mat &image, cv::Mat &transformed, cv::Rect rect,
                 cv::Mat &ldk);
  cv::Mat GetAlignmentMatrix(std::vector<cv::Point2f> &pts);
  float  GetL2Norm();
  std::string tag;

private:
  std::shared_ptr<ModelInfer> m_infer;
  float L2_norm;
};

#endif // LMKTRACKING_LIB_FEATURE_EXTRACT_H
