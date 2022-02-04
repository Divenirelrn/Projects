#ifndef FACETRACKING_LANDMARKTRACKING_H
#define FACETRACKING_LANDMARKTRACKING_H
#include "camera_stream.h"
#include "datareader.h"
#include "detector/face_detector_retina.h"
#include "detector/retinafacev2_mnn.h"
#include "face_info.h"
#include "log.h"
#include "model_infer/mnn_infer.h"
#include "model_loader.h"
#include "mtcnn.h"
#include "mtcnn_mnn.h"
#include "utils.h"
#include "zip_wrapper.h"
#include <opencv2/opencv.hpp>
#include <thread>

class LandmarkTracker {

public:
  LandmarkTracker(ZipWrapper *wrapper);
  LandmarkTracker();
  void Reset(ZipWrapper *wrapper);
  void DetectFace(const cv::Mat &input, float scale, SG::CameraStream &rgb_image, SG::CameraStream &ir_image);
  void SparseLandmarkPredict(const cv::Mat &raw_face_crop, std::vector<cv::Point2f> &landmarks_output, float &score, float size);
  bool TrackFace(SG::CameraStream &image, SG::CameraStream &ir_image, FaceInfo &face);
  bool TrackFaceMNN(SG::CameraStream &image, FaceInfo &face);
  void BlackingTrackingRegion(cv::Mat &image, cv::Rect &rect_mask);
  void UpdateStream(SG::CameraStream &image, SG::CameraStream &ir_image, bool is_detect);
  float CaculateIou(FaceInfo one, FaceInfo two);
  void MergeClosedBox(std::vector<FaceInfo>& trackingFace);
  std::vector<FaceInfo> trackingFace;

private:
  int min_face_size_;
  std::shared_ptr<MTCNN_MNN> face_detector_mnn;
  std::shared_ptr<MTCNN> face_detector_;
  std::shared_ptr<Detector> face_detector_retina_;
  std::shared_ptr<RetinaFacev2Mnn> face_retinafacev2_mnn;
  //std::vector<FaceInfo> candidate_faces_;

  int detection_index_;
  int detection_interval_;
  //ncnn::Net net_landmark_;
  int tracking_idx_;

  // std::shared_ptr<ModelInfer> ptr;
  //  Model Infer
  std::shared_ptr<ModelInfer> lmk_model_;
  int lmk_width_;
  int lmk_height_;
  float *mean;
  float *normal;
};

#endif // FACETRACKING_LANDMARKTRACKING_H
