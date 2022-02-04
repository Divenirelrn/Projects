#include "c_api/c_api.h"
#include "opencv2/opencv.hpp"
#include "utils/test_helper.h"
#include <string>
//#include "gtest/gtest.h"

std::vector<std::vector<cv::Point2f>> read_data(const std::string &filename) {
  std::ifstream infile;
  infile.open(filename);
  std::vector<std::vector<cv::Point2f>> pts;
  if (infile.eof()) {
    std::vector<cv::Point2f> pts_in;
    for (int i = 0; i < 106; i++) {
      float x = -1;
      float y = -1;
      infile >> x;
      infile >> y;
      cv::Point2f p(x, y);
      pts_in.push_back(p);
    }
    pts.push_back(pts_in);
  }
  return pts;
}

void write_data(const std::string &filename,
                const std::vector<std::vector<cv::Point2f>> &pts) {
  std::ofstream outfile;
  outfile.open(filename);
  for (auto pts_in : pts) {
    for (int i = 0; i < 106; i++) {
      cv::Point2f p = pts_in[i];
      outfile << p.x;
      outfile << p.y;
    }
  }
}


// void test() {
////TEST(FaceDetectorRetina4K, Base) {
//
//
//}

int main(int argc, char **argv) {

  char *model_path_ = argv[1];
  printf("model path: %s \n", model_path_);
  char *test_folder_path = argv[2];
  printf("test folder path: %s \n", test_folder_path);
  char *pic_nums = argv[3];
  int pic_num = atoi(pic_nums);
  printf("pic num: %d \n", pic_num);
  SG_CameraStream *cameraStream = SG_CreateCameraStream();
  SG_CameraStreamSetRotationMode(cameraStream, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(cameraStream, STREAM_YUV_NV21);
  std::string model_path(model_path_);
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_path.c_str());
  std::string base_folder(test_folder_path);
  double cost_time = 0;
  std::vector<std::vector<cv::Point2f>> pts_q;
  for (int i = 10000; i < pic_num + 10000; i++) {
    std::string filename = base_folder + "/" + std::to_string(i) + ".jpg";
    printf("read filename %s \n", filename.c_str());
    cv::Mat image = cv::imread(filename);
    TestUtils::rotate(image, image, SG::ROTATION_0);
    uint8_t *data = TestUtils::rgb2nv21(image);
    double timeStart = (double)cv::getTickCount();
    SG_CameraStreamSetData(cameraStream, data, image.cols, image.rows);
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, cameraStream);
    double nTime =
        (((double)cv::getTickCount() - timeStart) / cv::getTickFrequency()) *
        1000;
    cost_time += nTime;
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    printf("get face_num %d \n", face_num);
    for (int i = 0; i < face_num; i++) {
      std::vector<cv::Point2f> pts(106);
      int lmk_num = SG_LandmarkTrackerGetLandmarkNum(sgLandmarkTracker);
      auto *lmks = new float[lmk_num * 2];
      for (int i = 0; i < 106; i++) {
        pts[i].x = lmks[i * 2];
        pts[i].y = lmks[i * 2 + 1];
      }
      pts_q.push_back(pts);
      SG_LandmarkTrackerGetInfoLandmarks(sgLandmarkTracker, i, lmks);
      float score = SG_LandmarkTrackerGetInfoScore(sgLandmarkTracker, i);
      int track_id = SG_LandmarkTrackerGetInfoTrackId(sgLandmarkTracker, i);
      // DrawPointsOnImage(image, lmks , lmk_num*2);
      delete[] lmks;
    }
    delete[] data;
  }

  printf("pure infer , avg cost %.2fms \n", cost_time / pic_num);
  std::vector<std::vector<cv::Point2f>> pts_except = read_data("data.txt");
  std::cout << "pts_except.size()" << pts_except.size() << std::endl;
  if (pts_except.size() == pts_q.size()) {
    for (int i = 0; i < pts_except.size(); i++) {
      for (int j = 0; j < 106; j++) {
        if (std::abs(pts_q[i][j].x - pts_except[i][j].x) < 1e-3) {
          printf("pass\n");
        } else {
          exit(0);
        }
      }
    }

  } else {
    printf("error size\n ");
  }
}
