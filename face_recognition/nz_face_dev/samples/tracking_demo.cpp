#include "face_info.h"
#include "landmark_tracker.h"
#include "utils/test_helper.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "zip_wrapper.h"

void DrawPointsOnImage(cv::Mat &image, const cv::Rect rect, const std::vector<cv::Point2f> &points) {
  
  cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 1, 8, 0);
  int shift = 3;
  int factor = (1 << shift);
  for (auto lmk : points) {
    cv::circle(image, cv::Point(lmk.x * factor + 0.5, lmk.y * factor + 0.5), 1,
               cv::Scalar(255, 255, 0), 2, cv::LINE_AA, shift);
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("./tracking_demo path_model path_video\n");
    exit(0);
  }

  std::string model_path(argv[1]);
  std::string video_path(argv[2]);

  printf("model:%s\n", argv[1]);
  printf("video:%s\n", argv[2]);
  //printf("model:%s", argv[1]);
  cv::VideoCapture video(video_path);
  std::cout<< "inputvideo sucess" <<std::endl;
  ZipWrapper zipWrapper(model_path);

  LandmarkTracker *detector = new LandmarkTracker(&zipWrapper);
  printf("new landmarktracker\n");
  cv::Mat image;
  video >> image;
  SG::CameraStream streaming;
  streaming.SetDataFormat(SG::NV21);
  streaming.SetRotationMode(SG::ROTATION_0);
  int num = 0;
  while (!image.empty()) {
    video >> image;
    num += 1;
    printf("-----------------------------------------\n");
    printf("process %d frame\n", num);
    printf("-----------------------------------------\n");

    TestUtils::rotate(image, image, SG::ROTATION_0);
    uint8_t *data = TestUtils::rgb2nv21(image);
    double timeStart = (double)cv::getTickCount();

    streaming.SetDataBuffer(data, image.rows, image.cols);
    detector->UpdateStream(streaming, streaming, false);
    double nTime =((double)cv::getTickCount() - timeStart) / cv::getTickFrequency();
    std::cout << nTime * 1000 << "ms" << std::endl;
    vector<FaceInfo> faces = detector->trackingFace;
    //printf("here, face.size:%d\n", faces.size());
    for (int i=0; i<faces.size(); i++) {
      FaceInfo face = faces[i];
      //printf("auto face\n");
      DrawPointsOnImage(image, face.GetRect(), face.GetLanmdark());
      string text  = to_string(face.GetTrackingId());
      string text1 = to_string(num);
      int font_face = cv::FONT_HERSHEY_COMPLEX;
      double font_scale = 2;
      int thickness = 2;
      int baseline;
      //printf("baseline\n");
      putText(image, text1, cv::Point(60, 60), font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);
      //printf("text1\n");
      putText(image, text, cv::Point(face.GetRect().x, face.GetRect().y), font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);
      //printf("text\n");
    }
    printf("-----------------------------------------\n");
    std::cout << "tracking " << faces.size()  <<" faces need time: " << nTime * 1000 << "ms"<< std::endl;
    printf("-----------------------------------------\n");

    string image_name ="/sdk/yangjunpei/nazhi_v5/test_images/track_result/"+ to_string(num) + ".jpg";
    cv::imwrite(image_name, image);
  }
}
