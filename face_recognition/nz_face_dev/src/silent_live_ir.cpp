//
// Created by YH-Mac on 2020/9/11.
//

#include "silent_live_ir.h"

#include <utility>

SilentLiveIR::SilentLiveIR() : m_models_num(2), m_scales{2.7, 4.0} {}

void SilentLiveIR::Reset(const std::string &models_path) {
//  float mean[] = {127.5, 127.5, 127.5};
//  float normal[] = {128, 128, 128};
//  //pr::Model *ir_model = model.ReadModel("ir_model");
//  m_infer_.reset(new ModelInfer(ir_model, 1, mean, normal));
//  m_infer_->Init("blob1", "relu13", 96, 96);
}

void SilentLiveIR::Reset(const pr::ModelLoader &model) {
  float mean[] = {127.5, 127.5, 127.5};
  float normal[] = {128, 128, 128};
  pr::Model *ir_model = model.ReadModel("ir_model");
  m_infer_.reset(new ModelInfer(ir_model, 1, mean, normal));

  std::vector<std::string> output_name;
  output_name.push_back("relu_blob13");

  m_infer_->Init("blob1", output_name, 96, 96);
}

SilentLiveIR::SilentLiveIR(const std::string &models_path) {
  Reset(models_path);
}

float SilentLiveIR::Check(const cv::Mat &crop_image) {
  assert(crop_image.rows == 96);
  assert(crop_image.cols == 96);
  
  std::vector<std::vector<float>> results = m_infer_->Infer(crop_image);
  std::vector<float> res = results[0];
  
  float _sum = 0;
  for (int i = 0; i < res.size(); i++)
    _sum += res[i];
  return _sum / 144.0;
}

