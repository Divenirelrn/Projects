//
// Created by YH-Mac on 2020/9/11.
//

#ifndef LMKTRACKING_LIB_SILENT_LIVE_IR_H
#define LMKTRACKING_LIB_SILENT_LIVE_IR_H

//#include "model_infer/opencv_infer.h"
#include "model_infer/mnn_infer.h"
#include "model_loader.h"

class SilentLiveIR {
public:
  SilentLiveIR();

  explicit SilentLiveIR(const std::string &models_path);
  void Reset(const std::string &models_path);
  void Reset(const pr::ModelLoader &model);
  float Check(const cv::Mat &crop_img);

private:
  std::shared_ptr<ModelInfer> m_infer_;
  int m_models_num;
  float m_scales[2];
};

#endif // LMKTRACKING_LIB_SILENT_LIVE_IR_H
