#ifndef __MTCNN_MNN_RNET_H__
#define __MTCNN_MNN_RNET_H__
//#include "net.h"
//#include <opencv2/opencv.hpp>
#include "datareader.h"
//#include "model_loader.h"
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
// using namespace cv;

#if 0
struct Bbox {
  float score;
  int x1;
  int y1;
  int x2;
  int y2;
  float area;
  float ppoint[10];
  float regreCoord[4];
};
#endif

class MTCNN_MNN {
public:
  MTCNN_MNN(ZipWrapper *wrapper);
  //MTCNN(const std::vector<std::string> param_files, const std::vector<std::string> bin_files);
  ~MTCNN_MNN();

  void SetMinFace(int minSize);
  //void detect(ncnn::Mat &img_, std::vector<Bbox> &finalBbox);
  //void detectMaxFace(ncnn::Mat &img_, std::vector<Bbox> &finalBbox);
  //void detection(const cv::Mat& img, std::vector<cv::Rect>& rectangles);
  //std::shared_ptr<ModelInfer> PnetInfer;
  std::shared_ptr<ModelInfer> RnetInfer;
  //std::shared_ptr<ModelInfer> OnetInfer;
private:
  //void generateBbox(ncnn::Mat score, ncnn::Mat location, vector<Bbox> &boundingBox_, float scale);
  //void nmsTwoBoxs(vector<Bbox> &boundingBox_, vector<Bbox> &previousBox_, const float overlap_threshold, string modelname = "Union");
  //void nms(vector<Bbox> &boundingBox_, const float overlap_threshold, string modelname = "Union");
  //void refine(vector<Bbox> &vecBbox, const int &height, const int &width, bool square);
  //void extractMaxFace(vector<Bbox> &boundingBox_);

  //void PNet(float scale);
  //void PNet();
  //void RNet();
  //void ONet();
  //ncnn::Mat img;
  //const float nms_threshold[3] = {0.5f, 0.7f, 0.7f};
  const float mean_vals[3]     = {127.5, 127.5, 127.5};
  const float norm_vals[3]     = {0.0078125, 0.0078125, 0.0078125};
  const int MIN_DET_SIZE       = 12;
  //std::vector<Bbox> firstPreviousBbox_, secondPreviousBbox_,
  //    thirdPrevioussBbox_;
  //std::vector<Bbox> firstBbox_, secondBbox_, thirdBbox_;
  //int img_w, img_h;

private:
  const float threshold[3] = {0.6f, 0.7f, 0.6f};
  int minsize = 40;
  //const float pre_facetor = 0.7090f;
};

#endif //__MTCNN_MNN_RNET_H__
