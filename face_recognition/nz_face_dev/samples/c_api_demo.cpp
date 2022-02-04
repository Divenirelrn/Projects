#include "c_api/c_api.h"
#include "opencv2/opencv.hpp"
#include "utils/test_helper.h"
#include <string>

void DrawPointsOnImage(cv::Mat &image, float *pts, int size) {
  int shift = 3;
  int factor = (1 << shift);
  int pts_size = size / 2;

  for (int i = 0; i < pts_size; i++) {
    cv::Point pt(pts[i * 2] * factor, pts[i * 2 + 1] * factor);
    cv::circle(image, pt, 1, cv::Scalar(255, 255, 0), 2, cv::LINE_AA, shift);
  }
}

int main(int argc, char **argv) {
  // cv::VideoCapture video("/Users/yujinke/Downloads/1591694255159319.mp4");
  std::string _data(argv[2]);
  cv::VideoCapture video;
  if (_data == "cam")
    video.open(0);
  else
    video.open(_data);
  SG_CameraStream *cameraStream = SG_CreateCameraStream();
  cv::Mat image;
  video >> image;
  cv::resize(image, image, cv::Size(), 0.5, 0.5);
  SG_CameraStreamSetRotationMode(cameraStream, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(cameraStream, STREAM_YUV_NV21);
  std::string model_path(argv[1]);
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_path.c_str());
  while (!image.empty()) {
    video >> image;
    TestUtils::rotate(image, image, SG::ROTATION_0);
    uint8_t *data = TestUtils::rgb2nv21(image);
    SG_CameraStreamSetData(cameraStream, data, image.cols, image.rows);
    double timeStart = (double)cv::getTickCount();
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, cameraStream,false);
    double nTime =
        ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
    std::cout << nTime * 1000 << "ms" << std::endl;
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
    // std::cout << "face nums:" << faces.size() << std::endl;
    cv::imshow("image", image);
    cv::waitKey(1);
  }
}
