//
// Created by YH-Mac on 2020/9/10.
//

#include "face_attribute.h"

#include "mtcnn.h"
#define WIDTH 96
#define HEIGHT 96

FaceAttribute::FaceAttribute() {}

void FaceAttribute::Reset(const std::string& model_path) {
  float mean[] = {0.0f, 0.0f, 0.0f};
  float normal[] = {1.0f, 1.0f, 1.0f};
  m_infer_.reset(new ModelInfer(model_path, 1, mean, normal));
  
  std::vector<std::string> output_name;
  //std::string out_name = "fc1";
  output_name.push_back("fc1");
  m_infer_->Init("data", output_name, WIDTH, HEIGHT);
}

FaceAttribute::FaceAttribute(const std::string& model_path) { Reset(model_path); }

void FaceAttribute::Extract(const cv::Mat &image, const cv::Rect2f &rect) {
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
  cv::Mat dst;
  cv::warpAffine(image, dst, m, cv::Size(96, 96));

  std::vector<std::vector<float>> results = m_infer_->Infer(dst);
  std::vector<float> res = results[0];

  gender_ = GENDER(ArgMax<float>(res, 0, 2));
  age_ = static_cast<int>((res[2] * 100));
  emotion_ = ArgMax<float>(res, 3, 10);
  ethnic_ = ArgMax<float>(res, 10, 14);
  glass_ = ArgMax<float>(res, 14, 17);

  //    157       gender = np.argmax(attr_pred[0:2])
  //    158       age = int(attr_pred[2]*100)
  //    159       ethn = np.argmax(attr_pred[3:7])
  //    160       glass = np.argmax(attr_pred[7:9])
  //    161       emotion = np.argmax(attr_pred[9:16])
}

float FaceAttribute::getQuality() const { return quality_; }

GENDER FaceAttribute::getGender() const { return gender_; }

int FaceAttribute::getAge() const { return age_; }

int FaceAttribute::getEthnic() const { return ethnic_; }

int FaceAttribute::getGlass() const { return glass_; }

int FaceAttribute::getEmotion() const { return emotion_; }
