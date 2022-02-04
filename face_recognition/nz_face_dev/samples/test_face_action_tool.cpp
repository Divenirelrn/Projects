#include "c_api/c_api.h"
#include "opencv2/opencv.hpp"
#include "utils/test_helper.h"
#include <string>
//#include <iostream>
#include <fstream>

const std::vector<int> idx{
    56, 95, 60, 35, 4,  2,  13, 54, 68, 106, 28, 21, 42,  44, 105, 52, 86,
    48, 20, 29, 30, 84, 85, 63, 49, 80, 59,  45, 75, 71,  72, 74,  55, 76,
    23, 47, 36, 92, 91, 93, 32, 94, 46, 46,  39, 37, 104, 33, 11,  14, 1};

void DrawPointsOnImage(cv::Mat &image, float *pts, int size) {
  int shift = 3;
  int factor = (1 << shift);
  int pts_size = size / 2;

  for (int i = 0; i < idx.size(); i++) {
    cv::Point pt(pts[(idx[i] - 1) * 2] * factor,
                 pts[(idx[i] - 1) * 2 + 1] * factor);
    cv::circle(image, pt, 1, cv::Scalar(255, 255, 0), 2, cv::LINE_AA, shift);
  }
}

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << argv[0] << " ";
    std::cout << "model_dir mp4_path result_path" << std::endl;
    return 1;
  }

  std::string model_dir(argv[1]);
  std::string mp4_path(argv[2]);
  std::string result_path(argv[3]);

  cv::VideoCapture video(0);
  // cv::VideoCapture video(mp4_path);
  SG_CameraStream *cameraStream = SG_CreateCameraStream();
  SG_CameraStreamSetRotationMode(cameraStream, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(cameraStream, STREAM_BGR);
  std::string model_path = model_dir + "/model.bin";
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_path.c_str());
  std::ofstream txt(result_path);
  int index = 0;
  while (1) {
    cv::Mat image;
    if (!video.read(image))
      break;
    uint8_t *data = image.data;
    SG_CameraStreamSetData(cameraStream, data, image.cols, image.rows);
    double timeStart = (double)cv::getTickCount();
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, cameraStream, false);
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    for (int i = 0; i < face_num; i++) {
      int lmk_num = SG_LandmarkTrackerGetLandmarkNum(sgLandmarkTracker);
      auto *lmks = new float[lmk_num * 2];
      SG_LandmarkTrackerGetInfoLandmarks(sgLandmarkTracker, i, lmks);
      float score = SG_LandmarkTrackerGetInfoScore(sgLandmarkTracker, i);
      int track_id = SG_LandmarkTrackerGetInfoTrackId(sgLandmarkTracker, i);
      float angle[3];
      SG_LandmarkTrackerGetInfoEulerAngle(sgLandmarkTracker, i, angle, 3);
      std::cout << "pitch:" << angle[0] << " yaw:" << angle[1]
                << " roll:" << angle[2] << std::endl;
      DrawPointsOnImage(image, lmks, lmk_num * 2);
      delete[] lmks;
    }
    double nTime =
        ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
    std::cout << nTime * 1000 << "ms" << std::endl;
    txt << index << " " << nTime * 1000 << std::endl;
    cv::imshow("image", image);
    cv::waitKey(1);
    index += 1;
  }
  txt.close();
  return 0;
}
