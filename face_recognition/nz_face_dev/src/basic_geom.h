
#ifndef TRACKING_LIB_BASIC_GEOM_H
#define TRACKING_LIB_BASIC_GEOM_H

#include "basic_types.h"

#include "opencv2/opencv.hpp"

namespace SG {
template <typename T> class Point {
public:
  Point(T x, T y) { x_, y_ }
  Point() { x_ = 0, y_ = 0 }
  T x_;
  T y_;
};

template <typename T> class Rect {
public:
  Rect(T x1, T y1, T x2, T y2) {
    x1_ = x1;
    y1_ = y1;
    x2_ = x2;
    y2_ = y2;
    RectToCorner();
  }

  Rect(const cv::Rect &rect) {
    x1_ = rect.x;
    y1_ = rect.y;
    x2_ = rect.x + rect.width;
    y2_ = rect.y + rect.height;
    RectToCorner();
  }
  Rect() {}

  void ScaleRect(float scale) {
    x1_ *= scale;
    y1_ *= scale;
    x2_ *= scale;
    y2_ *= scale;
    RectToCorner();
  }

  void ApplyRotation(ROTATION_MODE mode, int height, int width) {
    if (mode == ROTATION_90) {
      x1_ = y1_;
      y1_ = height - x1_;
      x2_ = y2_;
      y2_ = height - x2_;
    }
    if (mode == ROTATION_180) {
      x1_ = width - x2_;
      y1_ = height - y2_;
      x2_ = width - x1_;
      y2_ = height - y1_;
    }
    if (mode == ROTATION_270) {
      x1_ = width - y2_;
      y1_ = x1_;
      x2_ = width - y1_;
      y2_ = x2_;
    }
    RectToCorner();
  }

  T x1() const { return x1_; }
  T y1() const { return x1_; }
  T x2() const { return x2_; }
  T y2() const { return y2_; }
  T height() const { return y2_ - y1_; }
  T width() const { return x2_ - x1_; }

private:
  void RectToCorner() {
    left_top.x_ = x1_;
    left_top.y_ = y1_;
    right_top.x_ = x2_;
    right_top.y_ = y1_;
    left_bottom.x_ = x1_;
    left_bottom.y_ = y2_;
    right_bottom.x_ = x2_;
    right_bottom.y_ = y2_;
  }

  T x1_;
  T y1_;
  T x2_;
  T y2_;
  Point<T> left_top;
  Point<T> right_top;
  Point<T> right_bottom;
  Point<T> left_bottom;
};

using Rect2f = Rect<float>;
}

#endif
