#include "c_api/c_api.h"
#include "opencv2/opencv.hpp"
#include "utils/test_helper.h"
#include <fstream>
#include <string>

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

std::string GenerateString(float *pts, int size) {
  int shift = 3;
  int factor = (1 << shift);
  int pts_size = size / 2;
  std::stringstream str;
  for (int i = 0; i < idx.size(); i++) {
    cv::Point2f pt(pts[(idx[i] - 1) * 2], pts[(idx[i] - 1) * 2 + 1]);
    str << pt.x << "," << pt.y << ",";
  }
  return str.str();
}

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << argv[0] << " ";
    std::cout << "model_dir image_path result_path" << std::endl;
    return 1;
  }

  std::string model_dir(argv[1]);
  std::string image_path(argv[2]);
  std::string result_path(argv[3]);

  SG_CameraStream *cameraStream = SG_CreateCameraStream();
  cv::Mat image = cv::imread(image_path);
  SG_CameraStreamSetRotationMode(cameraStream, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(cameraStream, STREAM_BGR);
  std::string model_path = model_dir + "/model.bin";
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_path.c_str());
  std::ofstream txt(result_path);
  txt << 106 << std::endl;
  txt << 0 << std::endl;
  uint8_t *data = image.data;
  SG_CameraStreamSetData(cameraStream, data, image.cols, image.rows);
  double all_time = 0;
  for (int i = 0; i < 100; i++) {
    double timeStart = (double)cv::getTickCount();
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, cameraStream, false);
    double nTime =
        ((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
    all_time += nTime * 1000;
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
      std::string generate = GenerateString(lmks, lmk_num);
      txt << image_path << "," << generate << nTime * 1000 << std::endl;
      delete[] lmks;
    }
  }
  txt << all_time << std::endl;
  txt.close();
  //    cv::imshow("image", image);
  //  cv::waitKey(0);
  return 0;
}
