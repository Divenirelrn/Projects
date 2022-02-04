//
// Created by YH-Mac on 2020/9/10.
//

#include "feature_extract.h"

#define WIDTH 112
#define HEIGHT 112

#include "mtcnn.h"
#include <math.h>

FeatureExtract::FeatureExtract() {}

void FeatureExtract::Reset(string model_path) {
  float mean[] = {0, 0, 0};
  float normal[] = {1, 1, 1};
  m_infer.reset(new ModelInfer(model_path, 1, mean, normal));

  std::vector<std::string> output_name;
  output_name.push_back("fc1");

  m_infer->Init("data", output_name, WIDTH, HEIGHT);
}

void FeatureExtract::Reset(const pr::ModelLoader &model) {
  //pr::Model *model_feature = model.ReadModel("y1");
  pr::Model *model_feature = model.ReadModel("y2");
  float mean[] = {0, 0, 0};
  float normal[] = {1, 1, 1};
  m_infer.reset(new ModelInfer(model_feature, 1, mean, normal));

  std::vector<std::string> output_name;
  output_name.push_back("fc1");

  m_infer->Init("data", output_name, WIDTH, HEIGHT);
}

// void FeatureExtract::Reset(string model_path) {
//    float mean[] = {127.5, 127.5, 127.5f};
//    float normal[] = {0.0078125, 0.0078125, 0.0078125};
//    m_infer.reset(new ModelInfer(model_path, 1, mean, normal));
//    m_infer->Init("data", "fc1_scale", WIDTH, HEIGHT);
//}
//
FeatureExtract::FeatureExtract(string model_path) { Reset(model_path); }

static int N = 0;
vector<float> FeatureExtract::Extract(cv::Mat &image) {
  cv::Mat image_bgr;
  //image = cv::imread("/data/jack_ssd/home/jack/codebase/face_sdk_dev/source/build/tmp/4079.jpg.png" );
 //image = cv::imread("tmp/" + tag + ".png");
  cv::cvtColor(image, image_bgr, cv::COLOR_BGR2RGB);
  cv::imwrite("tmp/" + tag + ".png", image);
  
  std::vector<vector<float>> results = m_infer->Infer(image_bgr);
  vector<float> infer_result = results[0];

  float l2 = 0;
  for (auto &one : infer_result) {
    l2 += one * one;
  }
  l2 = sqrt(l2);
  L2_norm = l2;
  std::cout<<"L2 norm:"<<l2<<std::endl;
  for (int i = 0; i < infer_result.size(); i++) {
    infer_result[i] /= l2;
  }

//  for(int i = 0 ; i < 10  ;i++) {
//      std::cout<<infer_result[i];
//  }
//  std::cout<<std::endl;
  //  cv::Mat to_mat = cv::Mat(infer_result);
  //  double dot = to_mat.dot(to_mat);
  //  cv::Mat feature = to_mat / sqrt(dot);
  //  vector<float> feature_vec = vector<float>(feature);

  return infer_result;
}

void FeatureExtract::Extract(cv::Mat &images, cv::Mat &feature) {
  cv::Mat image_bgr;
  //cv::imwrite("tmp/" + tag + ".png", image_bgr);

  cv::cvtColor(images, image_bgr, cv::COLOR_BGR2RGB);
  // cv::Mat image_bgr = cv::cvtColor(images, cv::COLOR_BGR2RGB);

  std::vector<vector<float>> results = m_infer->Infer(image_bgr);
   vector<float> infer_result = results[0];

  cv::Mat to_mat = cv::Mat(infer_result);
  double dot = to_mat.dot(to_mat);
  feature = to_mat / sqrt(dot);
}

void FeatureExtract::Alignment(cv::Mat &image, cv::Mat &transformed,
                               cv::Rect rect, cv::Mat &ldk) {
  float src_pts[] = {30.2946, 51.6963, 65.5318, 51.5014, 48.0252,
                     71.7366, 33.5493, 92.3655, 62.7299, 92.2041};
  int w = rect.width;
  int h = rect.height;
  cv::Size image_size(112, 112);
  cv::Size bounding_box_size(w, h);
  if (image_size.height) {
    for (int i = 0; i < 5; i++) {
      *(src_pts + 2 * i) += 8.0;
    }
  }
  cv::Mat src(5, 2, CV_32F);
  src.data = (uchar *)src_pts;
  cv::Mat M_temp = FacePreprocess::similarTransform(ldk, src);
  cv::Mat M = M_temp.rowRange(0, 2);
  cv::warpAffine(image, transformed, M, image_size, cv::INTER_CUBIC);
}

cv::Mat FeatureExtract::GetAlignmentMatrix(std::vector<cv::Point2f> &pts) {
  float src_pts[] = {30.2946, 51.6963, 65.5318, 51.5014, 48.0252,
                     71.7366, 33.5493, 92.3655, 62.7299, 92.2041};
  cv::Size image_size(112, 112);
  cv::Mat input_mat(5, 2, CV_32F, pts.data());
  // cv::Mat input_mat(5,2,CV_32F,pts.data());
  if (image_size.height) {
    for (int i = 0; i < 5; i++) {
      *(src_pts + 2 * i) += 8.0;
    }
  }
  cv::Mat src(5, 2, CV_32F);
  src.data = (uchar *)src_pts;
  cv::Mat M_temp = FacePreprocess::similarTransform(input_mat, src);
  cv::Mat M = M_temp.rowRange(0, 2);
  std::cout << "[AFFINE]:" << M.at<float>(0, 0) << " " << M.at<float>(0, 1)
            << " " << M.at<float>(0, 2) << " " << M.at<float>(1, 0) << " "
            << M.at<float>(1, 1) << " " << M.at<float>(1, 2) << std::endl;
  return M;
}

float  FeatureExtract::GetL2Norm(){
  return L2_norm;
}
