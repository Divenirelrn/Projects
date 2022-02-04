#ifndef LMKTRACKING_LIB_FACE_ACTION_H
#define LMKTRACKING_LIB_FACE_ACTION_H
#include "context_config.h"
#include "opencv2/opencv.hpp"
#include "utils.h"

enum FACE_ACTION {
  NORMAL = 0,
  SHAKE = 0,
  BROW_UP = 1,
  BLINK = 2,
  JAW_OPEN = 3,
  HEAD_RISE = 4
};

class FaceAction {
public:
  FaceAction(int record_list_length) {
    record_list.resize(record_list_length);
    record_list_euler.resize(record_list_length);
    record_size = record_list_length;
    index = 0;
  }

  void RecordActionFrame(const std::vector<cv::Point2f> &landmark,
                         const cv::Vec3f &euler_angle) {
    MoveRecordList();
    record_list[0] = landmark;
    record_list_euler[0] = euler_angle;
    index += 1;
  }

  void Reset() {
    record_list.clear();
    record_list.resize(record_size);
    record_list_euler.clear();
    record_list_euler.resize(record_size);
    index = 0;
  }

  void AnalysisFaceAction() {
    actions.clear();
    eye_state_list.clear();
    if (index < record_list.size()) {
      actions.push_back(NORMAL);
    } else {
      // count eye aspect ratio
      for (int i = 0; i < record_list.size(); i++) {
        float left_eye_widthwise_d =
            PointDistance(record_list[i][CONST::left_eye_left_corner],
                          record_list[i][CONST::left_eye_right_corner]);
        float left_eye_heightwise_d =
            PointDistance(record_list[i][CONST::left_eyelid_upper],
                          record_list[i][CONST::left_eyelid_lower]);
        float eye_left_aspect_ratio =
            left_eye_heightwise_d / left_eye_widthwise_d;
        float right_eye_widthwise_d =
            PointDistance(record_list[i][CONST::right_eye_left_corner],
                          record_list[i][CONST::right_eye_right_corner]);
        float right_eye_heightwise_d =
            PointDistance(record_list[i][CONST::right_eyelid_upper],
                          record_list[i][CONST::right_eyelid_lower]);
        float eye_right_aspect_ratio =
            right_eye_heightwise_d / right_eye_widthwise_d;
        std::pair<float, float> eye_state(eye_left_aspect_ratio,
                                          eye_right_aspect_ratio);
        eye_state_list.push_back(eye_state);
      }

      // count mouth aspect ratio
      float mouth_widthwise_d =
          PointDistance(record_list[0][CONST::mouth_left_corner],
                        record_list[0][CONST::mouth_right_corner]);
      float mouth_heightwise_d =
          PointDistance(record_list[0][CONST::mouth_upper],
                        record_list[0][CONST::mouth_lower]);
      float mouth_aspect_ratio = mouth_heightwise_d / mouth_widthwise_d;
      if (mouth_aspect_ratio > 0.3) {
        std::cout << "ACTION:JAW_OPEN" << std::endl;
        actions.push_back(JAW_OPEN);
      }

      int counter_eye_open = 0;
      int counter_eye_close = 0;
      for (auto &e : eye_state_list) {
        if (e.first < 0.25 || e.second < 0.25) {
          counter_eye_close += 1;
        }
        if (e.first > 0.25 || e.second > 0.25) {
          counter_eye_open += 1;
        }
      }
      if (counter_eye_close > 0 && counter_eye_open > 2 &&
          record_list_euler[0][1] > -6 && record_list_euler[0][0] < 6) {
        actions.push_back(BLINK);
        std::cout << "ACTION:BLINK" << std::endl;
        Reset();
      }

      bool counter_head_shake_left = false;
      bool counter_head_shake_right = false;
      for (auto &e : record_list_euler) {
        if (e[1] < -6) {
          counter_head_shake_left = true;
        }
        if (e[1] > 6) {
          counter_head_shake_right = true;
        }
      }
      if (counter_head_shake_left && counter_head_shake_right) {
        actions.push_back(SHAKE);
        std::cout << "ACTION:SHAKE" << std::endl;
      }

      if (record_list_euler[0][0] < -10) {
        actions.push_back(HEAD_RISE);
        std::cout << record_list_euler[0][0] << std::endl;
      }
    }
  }

private:
  void MoveRecordList() {
    // for(int i = 0 ; i < record_list.size() - 1 ; i++){
    //    record_list[i+1] = record_list[i];
    //    record_list_euler[i+1] = record_list_euler[i];
    //}
    for (int i = record_list.size() - 1; i > 0; i--) {
      record_list[i] = record_list[i - 1];
      record_list_euler[i] = record_list_euler[i - 1];
    }
  }
  std::vector<std::vector<cv::Point2f>> record_list;
  std::vector<cv::Vec3f> record_list_euler;
  std::vector<std::pair<float, float>> eye_state_list; // pair  left right
  std::vector<float> mouth_state_list;
  std::vector<FaceAction> actions;
  int record_size;
  int index;
};
#endif // LMKTRACKING_LIB_FACE_ACTION_H
