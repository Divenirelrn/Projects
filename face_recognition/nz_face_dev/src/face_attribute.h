//
// Created by YH-Mac on 2020/9/10.
//

#ifndef LMKTRACKING_LIB_FACE_ATTRIBUTE_H
#define LMKTRACKING_LIB_FACE_ATTRIBUTE_H
#include "camera_stream.h"
#include "face_preprocess.h"
#include "log.h"
#include "model_infer/mnn_infer.h"
#include "utils.h"
#include <opencv2/opencv.hpp>
enum GENDER { FEMALE, MALE };
// enum ethn{};

class FaceAttribute {
public:
  FaceAttribute();
  /**
   * 初始化
   * @param model_path
   * */
  FaceAttribute(const std::string& model_path);
  void Reset(const std::string& model_path);
  void Extract(const cv::Mat &image, const cv::Rect2f &rect);

private:
  std::shared_ptr<ModelInfer> m_infer_;
  float quality_;
  GENDER gender_;
  int age_;

public:
  float getQuality() const;

  GENDER getGender() const;

  int getAge() const;

  int getEthnic() const; //0=white, 1=asian, 2=black, 3=india

  int getGlass() const; //0=none, 1=normal, 2=dark

  int getEmotion() const; //order = ['anger', 'disgust', 'fear', 'happiness', 'neutral', 'sadness', 'surprise']

private:
  int ethnic_;
  int glass_;
  int emotion_;

  //    157       gender = np.argmax(attr_pred[0:2])
  //    158       age = int(attr_pred[2]*100)
  //    159       ethn = np.argmax(attr_pred[3:7])
  //    160       glass = np.argmax(attr_pred[7:9])
  //    161       emotion = np.argmax(attr_pred[9:16])
};

#endif // LMKTRACKING_LIB_FACE_ATTRIBUTE_H
