#ifndef __RETINAFACEV2_MNN_H__
#define __RETINAFACEV2_MNN_H__

#include <opencv2/opencv.hpp>
#include "datareader.h"
#include "zip_wrapper.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include <vector>
#include "model_infer/mnn_infer.h"
#include "model_loader.h"

using namespace std;
using namespace cv;

typedef struct BoxInfo {
  float x1;
  float y1;
  float x2;
  float y2;
  float score;
  int   label;
  float cls_score;
  int   cls_idx;
} BoxInfo;

class RetinaFacev2Mnn {
public:
  RetinaFacev2Mnn(ZipWrapper *wrapper, std::string model_file);
  RetinaFacev2Mnn();
  int reset(ZipWrapper *wrapper, std::string model_file);
  ~RetinaFacev2Mnn();

  int Detect(const cv::Mat &image, std::vector<BoxInfo>& boxes);


private:
  void nms(std::vector<BoxInfo> &input_boxes, float NMS_THRESH);
  
  

private:
  std::shared_ptr<ModelInfer> retinafacev2_infer;
  int       in_height;
  int       in_width;
  float     nms_threshold;
  float     score_threshold;
};

#endif //__RETINAFACEV2_MNN_H__
