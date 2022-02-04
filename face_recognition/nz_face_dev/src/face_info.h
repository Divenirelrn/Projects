#ifndef FACE_INFO_H
#define FACE_INFO_H
#include <memory>

#include "face_action.h"
#include "opencv2/opencv.hpp"
#include "utils.h"

enum TRACK_STATE { UNTRACKING = -1, DETECT = 0, READY = 1, TRACKING = 2 };

class FaceInfo {
public:
  FaceInfo(int instance_id, cv::Rect bbox, float scale, int num_landmark = 106) {
    face_id_        = instance_id;
    landmark_.resize(num_landmark);
    bbox_           = bbox;
    tracking_state_ = DETECT;
    confidence_     = 1.0;
    face_action_    = std::make_shared<FaceAction>(10);
    best_rgb_buffer = nullptr;
    best_ir_buffer  = nullptr;
    rgb_size        = 0;
    ir_size         = 0;
    
    scale_det       = scale;
    continous_times = 0;
    best_bbox       = bbox;
    min_angle       = 180.f;
  }

#if 0
  ~FaceInfo() {
    if (best_rgb_buffer != nullptr) {
      free(best_rgb_buffer);
      best_rgb_buffer = nullptr;
    }
    if (best_ir_buffer != nullptr) {
      free(best_ir_buffer);
      best_ir_buffer = nullptr;
    }
  }
#endif

#if 0
  FaceInfo & FaceInfo::operator=(FaceInfo & faceinfo)
{
	if (this == &faceinfo){
		return *this;
	}
  
	rgb_size = faceinfo.rgb_size;
  ir_size  = faceinfo.ir_size;	
	if (best_rgb_buffer != NULL){
		delete[] best_rgb_buffer;
		best_rgb_buffer = NULL;
	}
  if (best_ir_buffer != NULL){
		delete[] best_ir_buffer;
		best_ir_buffer = NULL;
	}

  best_rgb_buffer = new unsigned char[rgb_size];
	memcpy(best_rgb_buffer, faceinfo.best_rgb_buffer, rgb_size);

  best_ir_buffer = new unsigned char[ir_size];
	memcpy(best_ir_buffer, faceinfo.best_ir_buffer, ir_size);
 
	return *this;
}

FaceInfo::FaceInfo(const FaceInfo &copy)
{
	rgb_size = copy.rgb.size;
  ir_size  = copy.ir_size;

	best_rgb_buffer = new unsigned char[rgb_size];
	memcpy(best_rgb_buffer, faceinfo.best_rgb_buffer, rgb_size);

  best_ir_buffer = new unsigned char[ir_size];
	memcpy(best_ir_buffer, faceinfo.best_ir_buffer, ir_size); 
}
#endif

  void SetFaceImageData(SG::CameraStream &rgb_image, SG::CameraStream &ir_image) {
    
    double timeStart = (double)cv::getTickCount();
    if (best_rgb_buffer == nullptr) {
      printf("create faceinfo malloc rgb data memory:size:%d\n", rgb_image.buffer_size_);
      best_rgb_buffer = (unsigned char*)malloc(rgb_image.buffer_size_);
      memcpy(best_rgb_buffer, rgb_image.buffer_, rgb_image.buffer_size_);
      rgb_size = rgb_image.buffer_size_;
    }
    if (best_ir_buffer == nullptr) {
      printf("create faceinfo malloc ir data memory:size:%d\n", ir_image.buffer_size_);
      best_ir_buffer = (unsigned char*)malloc(ir_image.buffer_size_);
      memcpy(best_ir_buffer, ir_image.buffer_, ir_image.buffer_size_);
      ir_size = ir_image.buffer_size_;
    }
    double nTime = ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
    printf("set face image data memcpy need time:%f ms\n",  nTime * 1000);

    return;
  }

  void UpdateMatrix(const cv::Mat &matrix) {
    assert(trans_matrix_.rows == 2 && trans_matrix_.cols == 3);
    double a00 = matrix.at<double>(0, 0);
    double a01 = matrix.at<double>(0, 1);
    double a10 = matrix.at<double>(1, 0);
    double a11 = matrix.at<double>(1, 1);
    double t1x = matrix.at<double>(0, 2);
    double t1y = matrix.at<double>(1, 2);

    double m00 = trans_matrix_.at<double>(0, 0);
    double m01 = trans_matrix_.at<double>(0, 1);
    double m10 = trans_matrix_.at<double>(1, 0);
    double m11 = trans_matrix_.at<double>(1, 1);
    double t0x = trans_matrix_.at<double>(0, 2);
    double t0y = trans_matrix_.at<double>(1, 2);

    double n_m00 = a00 * m00 + a01 * m10;
    double n_m01 = a00 * m01 + a01 * m11;
    double n_m02 = a00 * t0x + a01 * t0y + t1x;
    double n_m10 = a10 * m00 + a11 * m10;
    double n_m11 = a10 * m01 + a11 * m11;
    double n_m12 = a10 * t0x + a11 * t0y + t1y;

    trans_matrix_.at<double>(0, 0) = n_m00;
    trans_matrix_.at<double>(0, 1) = n_m01;
    trans_matrix_.at<double>(0, 2) = n_m02;
    trans_matrix_.at<double>(1, 0) = n_m10;
    trans_matrix_.at<double>(1, 1) = n_m11;
    trans_matrix_.at<double>(1, 2) = n_m12;
  }

  void SetLandmarkAndBestFrameInfo(const std::vector<cv::Point2f> &lmk, float confidence, SG::CameraStream &rgb_image, SG::CameraStream &ir_image, bool update_rect = true, bool update_matrix = true) {
    assert(lmk.size() == landmark_.size());
    std::copy(lmk.begin(), lmk.end(), landmark_.begin());
    DynamicSmoothParamUpdate(landmark_, landmark_smooth_aux_, 106 * 2, 0.06);

    //cv::Vec3f cur_angle;
    // cv::Vec3d euler_angle;
    EstimateHeadPose(landmark_, euler_angle_);
    // DynamicSmoothParamUpdate(landmark_, landmark_smooth_aux_, 106 * 2, 0.06);
    if (update_rect) {
      bbox_ = cv::boundingRect(lmk);
    }
    printf("cv::boundingrect x1:%d, y1:%d, x2:%d, y2:%d\n", bbox_.x, bbox_.y, bbox_.x+bbox_.width, bbox_.y+bbox_.height);      
    float max_pre_angle = (fabs(euler_angle_[0]) > fabs(euler_angle_[1])) ? std::max(fabs(euler_angle_[0]), fabs(euler_angle_[2])) : std::max(fabs(euler_angle_[1]), fabs(euler_angle_[2]));
    //float max_cur_angle = (fabs(cur_angle[0]) > fabs(cur_angle[1])) ? std::max(fabs(cur_angle[0]), fabs(cur_angle[2])) : std::max(fabs(cur_angle[1]), fabs(cur_angle[2]));
    if (max_pre_angle < min_angle-10 && confidence >= CONST::mtcnn_rnet_threshold) {
      min_angle        = max_pre_angle;
      best_bbox.x      = bbox_.x * scale_det;
      best_bbox.y      = bbox_.y * scale_det;
      best_bbox.width  = bbox_.width * scale_det;
      best_bbox.height = bbox_.height * scale_det;
      //best_bbox    = bbox_;
      
      double timeStart = (double)cv::getTickCount();
      if (best_rgb_buffer != nullptr && best_ir_buffer != nullptr){
        memcpy(best_rgb_buffer, rgb_image.buffer_, rgb_image.buffer_size_);
        memcpy(best_ir_buffer, ir_image.buffer_, ir_image.buffer_size_);
        rgb_size = rgb_image.buffer_size_;
        ir_size  = ir_image.buffer_size_;
      }
      double nTime = ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
      printf("update best frame need time:%f ms, angle:%f\n",  nTime * 1000, min_angle);
    }

    if (update_matrix && tracking_state_ == TRACKING) {
      // pass
    }
  }

  std::vector<cv::Point2f> GetLanmdark() const { return landmark_; }

  cv::Rect GetRect() const { return bbox_; }

  cv::Rect GetRectSquare(float padding_ratio = 0.0) const {
    int cx = bbox_.x + bbox_.width / 2;
    int cy = bbox_.y + bbox_.height / 2;
    int R = std::max(bbox_.width, bbox_.height) / 2;
    int R_padding = static_cast<int>(R * (1 + padding_ratio));
    int x1 = cx - R_padding;
    int y1 = cy - R_padding;
    int x2 = cx + R_padding;
    int y2 = cy + R_padding;
    int width = x2 - x1;
    int height = y2 - y1;
    assert(width > 0);
    assert(height > 0);
    assert(height == width);
    // have wrong problem, height==width,0<x1<

    cv::Rect box_square(x1, y1, width, height);
    return box_square;
  }

  void UpdateFaceAction() {
    face_action_->RecordActionFrame(landmark_, euler_angle_);
    face_action_->AnalysisFaceAction();
  }

  void DisableTracking() { tracking_state_ = UNTRACKING; }

  void EnableTracking() { tracking_state_ = TRACKING; }

  void ReadyTracking() { tracking_state_ = READY; }

  TRACK_STATE TrackingState() const { return tracking_state_; }

  float GetConfidence() const { return confidence_; }

  void SetConfidence(float confidence) { confidence_ = confidence; }

  void SetContinousTimes(float score) {
    if (score >= CONST::mtcnn_rnet_threshold) {
      continous_times += 1;
    } else {
      continous_times = 0;
    }
  }

  int GetTrackingId() const { return face_id_; }

  const cv::Mat &getTransMatrix() const { return trans_matrix_; }

  void setTransMatrix(const cv::Mat &transMatrix) {
    transMatrix.copyTo(trans_matrix_);
  }

  static float L2norm(float x0, float y0, float x1, float y1) {
    return sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
  }

  void RequestFaceAction(
      std::vector<cv::Point2f> &landmarks,
      std::vector<std::vector<cv::Point2f>> &landmarks_lastNframes,
      int lm_length, float h) {
    int n = 5;
    std::vector<cv::Point2f> landmarks_temp;
    landmarks_temp.assign(landmarks.begin(), landmarks.end());
    if (landmarks_lastNframes.size() == n) {
      for (int i = 0; i < lm_length / 2; i++) {
        float sum_d = 1;
        float max_d = 0;
        for (int j = 0; j < n; j++) {
          float d = L2norm(landmarks_temp[i].x, landmarks_temp[i].y,
                           landmarks_lastNframes[j][i].x,
                           landmarks_lastNframes[j][i].y);
          if (d > max_d)
            max_d = d;
        }
        for (int j = 0; j < n; j++) {
          float d = exp(-max_d * (n - j) * h);
          sum_d += d;
          landmarks[i].x = landmarks[i].x + d * landmarks_lastNframes[j][i].x;
          landmarks[i].y = landmarks[i].y + d * landmarks_lastNframes[j][i].y;
        }
        landmarks[i].x = landmarks[i].x / sum_d;
        landmarks[i].y = landmarks[i].y / sum_d;
      }
    }
    std::vector<cv::Point2f> landmarks_frame;
    for (int i = 0; i < lm_length / 2; i++) {
      landmarks_frame.push_back(cv::Point2f(landmarks[i].x, landmarks[i].y));
    }
    landmarks_lastNframes.push_back(landmarks_frame);
    if (landmarks_lastNframes.size() > 5)
      landmarks_lastNframes.erase(landmarks_lastNframes.begin());
  }

  void DynamicSmoothParamUpdate(std::vector<cv::Point2f> &landmarks, std::vector<std::vector<cv::Point2f>> &landmarks_lastNframes, int lm_length, float h) {
    int n = 5;
    std::vector<cv::Point2f> landmarks_temp;
    landmarks_temp.assign(landmarks.begin(), landmarks.end());
    if (landmarks_lastNframes.size() == n) {
      for (int i = 0; i < lm_length / 2; i++) {
        float sum_d = 1;
        float max_d = 0;
        for (int j = 0; j < n; j++) {
          float d = L2norm(landmarks_temp[i].x, landmarks_temp[i].y, landmarks_lastNframes[j][i].x, landmarks_lastNframes[j][i].y);
          if (d > max_d)
            max_d = d;
        }
        for (int j = 0; j < n; j++) {
          float d = exp(-max_d * (n - j) * h);
          sum_d += d;
          landmarks[i].x = landmarks[i].x + d * landmarks_lastNframes[j][i].x;
          landmarks[i].y = landmarks[i].y + d * landmarks_lastNframes[j][i].y;
        }
        landmarks[i].x = landmarks[i].x / sum_d;
        landmarks[i].y = landmarks[i].y / sum_d;
      }
    }
    std::vector<cv::Point2f> landmarks_frame;
    for (int i = 0; i < lm_length / 2; i++) {
      landmarks_frame.push_back(cv::Point2f(landmarks[i].x, landmarks[i].y));
    }
    landmarks_lastNframes.push_back(landmarks_frame);
    if (landmarks_lastNframes.size() > 5) {
      landmarks_lastNframes.erase(landmarks_lastNframes.begin());
    }
  }

public:
  std::vector<cv::Point2f> landmark_;
  std::vector<std::vector<cv::Point2f>> landmark_smooth_aux_;
  cv::Rect bbox_;
  cv::Vec3f euler_angle_;
  const cv::Vec3f &getEulerAngle() const { return euler_angle_; }

  const cv::Rect &getBbox() const { return bbox_; }
  void setBbox(const cv::Rect &bbox) { bbox_ = bbox; }
  cv::Mat trans_matrix_;
  float confidence_;
  cv::Rect detect_bbox_;
  
  // just use euler angle to decide the best face for recognition 
  cv::Rect best_bbox;
  uint8_t *best_rgb_buffer;
  int rgb_size;
  uint8_t *best_ir_buffer;
  int ir_size;
  float min_angle;
  int face_id_;
  int continous_times;
  float scale_det;

private:
  //int continous_times;
  TRACK_STATE tracking_state_;
  std::shared_ptr<FaceAction> face_action_;
  //int face_id_;
};

#endif // FACE_INFO_H
