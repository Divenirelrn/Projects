//
// Created by YH-Mac on 2020/9/11.
//

#ifndef LMKTRACKING_LIB_SILENT_LIVE_H
#define LMKTRACKING_LIB_SILENT_LIVE_H

#include "model_infer/mnn_infer.h"
#include "model_loader.h"

class SilentLive {
public:
  SilentLive();

  explicit SilentLive(const std::vector<std::string> &models_path);
  void Reset(const std::vector<std::string> &models_path);
  void Reset(const pr::ModelLoader &model);
  float check(const cv::Mat &img, cv::Rect &box, bool is_crop = true);
  // float check(const cv::Mat &img,const cv::Mat &img_ir,  cv::Rect &box, bool
  // is_crop = true);

private:
  bool CalculateBox(cv::Rect &box, int w, int h, float scale_or, cv::Rect& cbox);

  std::vector<std::shared_ptr<ModelInfer>> m_infers;
  int m_models_num;
  float m_scales[2];
};

#endif // LMKTRACKING_LIB_SILENT_LIVE_H
