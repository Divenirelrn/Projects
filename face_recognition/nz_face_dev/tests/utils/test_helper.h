
#ifndef TRACKING_LIB_TEST_HELPER_H
#define TRACKING_LIB_TEST_HELPER_H

#include "camera_stream.h"

namespace TestUtils {

inline uint8_t *rgb2nv21(const cv::Mat &Img) {
  if (Img.empty()) {
    exit(0);
  }
  int cols = Img.cols;
  int rows = Img.rows;

  int Yindex = 0;
  int UVindex = rows * cols;

  unsigned char *yuvbuff =
      new unsigned char[static_cast<int>(1.5 * rows * cols)];

  cv::Mat NV21(rows + rows / 2, cols, CV_8UC1);
  cv::Mat OpencvYUV;
  cv::Mat OpencvImg;
  cv::cvtColor(Img, OpencvYUV, cv::COLOR_BGR2YUV_YV12);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      uchar *YPointer = NV21.ptr<uchar>(i);
      int B = Img.at<cv::Vec3b>(i, j)[0];
      int G = Img.at<cv::Vec3b>(i, j)[1];
      int R = Img.at<cv::Vec3b>(i, j)[2];
      int Y = (77 * R + 150 * G + 29 * B) >> 8;
      YPointer[j] = Y;
      yuvbuff[Yindex++] = (Y < 0) ? 0 : ((Y > 255) ? 255 : Y);
      uchar *UVPointer = NV21.ptr<uchar>(rows + i / 2);
      if (i % 2 == 0 && (j) % 2 == 0) {
        int U = ((-44 * R - 87 * G + 131 * B) >> 8) + 128;
        int V = ((131 * R - 110 * G - 21 * B) >> 8) + 128;
        UVPointer[j] = V;
        UVPointer[j + 1] = U;
        yuvbuff[UVindex++] = (V < 0) ? 0 : ((V > 255) ? 255 : V);
        yuvbuff[UVindex++] = (U < 0) ? 0 : ((U > 255) ? 255 : U);
      }
    }
  }
  return yuvbuff;
}

inline void rotate(const cv::Mat &image, cv::Mat &out,
                   SG::ROTATION_MODE mode) {
  if (mode == SG::ROTATION_90) {
    cv::transpose(image, out);
    cv::flip(out, out, 2);
  } else if (mode == SG::ROTATION_180) {
    cv::flip(out, out, -1);
  } else if (mode == SG::ROTATION_270) {
    cv::transpose(image, out);
    cv::flip(out, out, 0);
  }
}

} // namespace TestUtils

#endif
