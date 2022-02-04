//
// Created by YH-Mac on 2020/9/11.
//

#include "silent_live.h"

#include <utility>

SilentLive::SilentLive() : m_models_num(2), m_scales{2.7, 4.0} {}

void SilentLive::Reset(const std::vector<std::string> &models_path) {
  assert(models_path.size() == 2);
  m_models_num = models_path.size();
  float mean[] = {0, 0, 0};
  float normal[] = {1, 1, 1};
  for (int i = 0; i < models_path.size(); ++i) {
    std::shared_ptr<ModelInfer> infer;
    infer.reset(new ModelInfer(models_path[i], 1, mean, normal));

    std::vector<std::string> output_name;
    output_name.push_back("softmax");

    infer->Init("input", output_name, 96, 96);
    m_infers.push_back(infer);
  };
  m_models_num = m_infers.size();
  m_scales[0] = 2.7;
  m_scales[1] = 4.0;
}

void SilentLive::Reset(const pr::ModelLoader &model) {
  m_infers.resize(2);
  m_models_num = m_infers.size();
  m_scales[0] = 2.7;
  m_scales[1] = 4.0;
  float mean[] = {0, 0, 0};
  float normal[] = {1, 1, 1};
  pr::Model *sfas_27 = model.ReadModel("sfas_27");
  pr::Model *sfas_40 = model.ReadModel("sfas_40");
  m_infers[0].reset(new ModelInfer(sfas_27, 1, mean, normal));

  std::vector<std::string> output_name;
  output_name.push_back("softmax");
  m_infers[0]->Init("input", output_name, 80, 80);

  m_infers[1].reset(new ModelInfer(sfas_40, 1, mean, normal));

  std::vector<std::string> output_name1;
  output_name1.push_back("softmax");
  m_infers[1]->Init("input", output_name1, 80, 80);
  m_models_num = 1; //use 2.7 model only
}

SilentLive::SilentLive(const std::vector<std::string> &models_path) {
  Reset(models_path);
}

float SilentLive::check(const cv::Mat &src, cv::Rect &box, bool is_crop) {
  float confidence = 0.f;
  for (int i = 0; i < m_models_num; ++i) {
    cv::Mat roi;
    if (is_crop) {
      cv::Rect rect;
      bool valid = CalculateBox(box, src.cols, src.rows, m_scales[i], rect);
      if(!valid) {
          return -1.0;
      }
      cv::resize(src(rect), roi, cv::Size(80, 80));
    } else {
      cv::resize(src, roi, cv::Size(80, 80));
    }
    
    std::vector<std::vector<float>> results = m_infers[i]->Infer(roi);
    std::vector<float> res = results[0];

    confidence += res[1];
  }
  confidence /= m_models_num;

  return confidence;
}

bool SilentLive::CalculateBox(cv::Rect &box, int w, int h, float scale_or, cv::Rect& cbox) {
  int box_width = box.width;
  int box_height = box.height;

  int shift_x = static_cast<int>(box_width * 0);
  int shift_y = static_cast<int>(box_height * 0);

  float scale = std::min(scale_or, std::min((w - 1) / (float)box_width,
                                            (h - 1) / (float)box_height));

  int box_center_x = box_width / 2 + box.x;
  int box_center_y = box_height / 2 + box.y;

  int new_width = static_cast<int>(box_width * scale);
  int new_height = static_cast<int>(box_height * scale);
  int new_size = std::max(new_width, new_height);
  int new_half = new_size / 2;
  int x1 = box_center_x - new_half;
  int x2 = box_center_x + new_half;
  int y1 = box_center_y - new_half;
  int y2 = box_center_y + new_half;
  int max_oob = 0;
  max_oob = std::max(max_oob, 0-x1);
  max_oob = std::max(max_oob, 0-y1);
  max_oob = std::max(max_oob, x2-w);
  max_oob = std::max(max_oob, y2-h);
  float oob_ratio = float(max_oob) / new_size;
  if(oob_ratio>0.2) return false;
  x1 = std::max(x1, 0);
  y1 = std::max(y1, 0);
  x2 = std::min(x2, w);
  y2 = std::min(y2, h);
  if(x1<=x2||y1<=y2) return false;
  cbox = cv::Rect(x1, y1, x2-x1, y2-y1);
  return true;


  //int left_top_x = box_center_x - new_width / 2 + shift_x;
  //int left_top_y = box_center_y - new_height / 2 + shift_y;
  //int right_bottom_x = box_center_x + new_width / 2 + shift_x;
  //int right_bottom_y = box_center_y + new_height / 2 + shift_y;

  //if (left_top_x < 0) {
  //  right_bottom_x -= left_top_x;
  //  left_top_x = 0;
  //}

  //if (left_top_y < 0) {
  //  right_bottom_y -= left_top_y;
  //  left_top_y = 0;
  //}

  //if (right_bottom_x >= w) {
  //  int s = right_bottom_x - w + 1;
  //  left_top_x -= s;
  //  right_bottom_x -= s;
  //}

  //if (right_bottom_y >= h) {
  //  int s = right_bottom_y - h + 1;
  //  left_top_y -= s;
  //  right_bottom_y -= s;
  //}

  //return cv::Rect(left_top_x, left_top_y, new_width, new_height);
}
