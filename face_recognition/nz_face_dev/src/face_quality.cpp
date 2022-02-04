//
// Created by YH-Mac on 2020/9/10.
//

#include "face_quality.h"
#include "mtcnn.h"
#define WIDTH 96
#define HEIGHT 96

FaceQuality::FaceQuality() {}

void FaceQuality::Reset(const std::string &model_path) {
  float mean[] = {0.0f, 0.0f, 0.0f};
  float normal[] = {1.0f, 1.0f, 1.0f};
  m_infer_.reset(new ModelInfer(model_path, 1, mean, normal));
  
  std::vector<std::string> output_name;
  output_name.push_back("fc1");

  m_infer_->Init("data", output_name, WIDTH, HEIGHT);
}

void FaceQuality::Reset(const pr::ModelLoader &model) {
  float mean[] = {0.0f, 0.0f, 0.0f};
  float normal[] = {1.0f, 1.0f, 1.0f};
  pr::Model *model_data = model.ReadModel("pose");
  m_infer_.reset(new ModelInfer(model_data, 1, mean, normal));

  std::vector<std::string> output_name;
  output_name.push_back("fc1");
  m_infer_->Init("data", output_name, WIDTH, HEIGHT);
}

cv::Mat FaceQuality::ComputeCropMatrix(const cv::Rect2f &rect) {
  float x = rect.x;
  float y = rect.y;
  float w = rect.width;
  float h = rect.height;
  float cx = x + w / 2;
  float cy = y + h / 2;
  float length = std::max(w, h) * 1.5 / 2;
  float x1 = cx - length;
  float y1 = cy - length;
  float x2 = cx + length;
  float y2 = cy + length;
  cv::Rect2f padding_rect(x1, y1, x2 - x1, y2 - y1);
  std::vector<cv::Point2f> rect_pts = Rect2Points(padding_rect);
  rect_pts.erase(rect_pts.end() - 1);
  std::vector<cv::Point2f> dst_pts = {{0, 0}, {96, 0}, {96, 96}};
  cv::Mat m = cv::getAffineTransform(rect_pts, dst_pts);
  return m;
}
void FaceQuality::Extract(const cv::Mat &image) {
  cv::Mat image_rgb;
  cv::cvtColor(image, image_rgb, cv::COLOR_BGR2RGB);

  std::vector<std::vector<float>> results = m_infer_->Infer(image_rgb);
  std::vector<float> res = results[0];
  
  pitch_ = res[0] * 90;
  yaw_ = res[1] * 90;
  roll_ = res[2] * 90;
  vector<float> quality(res.begin() + 13, res.end());
  quality_ = quality;
  std::vector<float> face_pts5(res.begin() + 3, res.begin() + 13);
  pts5_.resize(5);
  for (int i = 0; i < 5; i++) {
    pts5_[i].x = (face_pts5[i * 2] + 1) * (96 / 2);
    pts5_[i].y = (face_pts5[i * 2 + 1] + 1) * (96 / 2);
  }
}

std::vector<float> FaceQuality::getQuality() const { return quality_; }

float FaceQuality::getYaw() const { return yaw_; }

float FaceQuality::getRoll() const { return roll_; }

float FaceQuality::getPitch() const { return pitch_; }

std::vector<cv::Point2f> FaceQuality::getPoints() const { return pts5_; }
