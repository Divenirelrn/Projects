//
// Created by YH-Mac on 2020/9/10.
//

#ifndef LMKTRACKING_LIB_FACE_QUALITY_H
#define LMKTRACKING_LIB_FACE_QUALITY_H
#include "camera_stream.h"
#include "face_preprocess.h"
#include "log.h"
#include "model_infer/mnn_infer.h"
#include "utils.h"
#include <opencv2/opencv.hpp>

// enum ethn{};

class FaceQuality {
public:
  FaceQuality();
  /**
   * 初始化
   * @param model_path
   * */
  void Reset(const std::string &model_path);
  void Reset(const pr::ModelLoader &model);
  void Extract(const cv::Mat &image);
  cv::Mat ComputeCropMatrix(const cv::Rect2f &rect);
  std::vector<float> getQuality() const;
  float getYaw() const;
  float getRoll() const;
  float getPitch() const;
  std::vector<cv::Point2f> getPoints() const;

private:
  std::shared_ptr<ModelInfer> m_infer_;
  std::vector<float> quality_;
  std::vector<cv::Point2f> pts5_;
  float yaw_;
  float roll_;
  float pitch_;

  //    157       gender = np.argmax(attr_pred[0:2])
  //    158       age = int(attr_pred[2]*100)
  //    159       ethn = np.argmax(attr_pred[3:7])
  //    160       glass = np.argmax(attr_pred[7:9])
  //    161       emotion = np.argmax(attr_pred[9:16])
};

#endif // LMKTRACKING_LIB_FACE_QUALITY_H
