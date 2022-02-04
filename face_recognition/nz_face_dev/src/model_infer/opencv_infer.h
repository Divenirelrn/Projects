#ifndef MODEL_INFER_OPENCV_INFER_H
#define MODEL_INFER_OPENCV_INFER_H

#include "model_loader.h"
#include "opencv2/opencv.hpp"

class ModelInferOCV {
public:
  ModelInferOCV(const std::string &model, int thread, float mean[],
                float normal[], bool use_model_bin = false) {
    model_ =
        cv::dnn::readNetFromCaffe(model + ".prototxt", model + ".caffemodel");
    for (int i = 0; i < 3; i++) {
      this->mean[i] = mean[i];
      this->normal[i] = normal[i];
    }
  }

  ModelInferOCV(const pr::Model *model, int thread, float mean[],
                float normal[]) {
    model_ = cv::dnn::readNetFromCaffe(
        model->prototxtBuffer, model->modelsize.prototxt_size,
        model->caffemodelBuffer, model->modelsize.caffemodel_size);
    for (int i = 0; i < 3; i++) {
      this->mean[i] = mean[i];
      this->normal[i] = normal[i];
    }
    // cv::setNumThreads(thread);
  }

  // ModelInfer(){}

  void Init(const std::string &input, const std::string &output, int width,
            int height) {
    input_node_ = input;
    output_node_ = output;
    width_ = width;
    height_ = height;
  }

  std::vector<float> Infer(const cv::Mat &mat) {
    assert(mat.rows == height_);
    assert(mat.cols == width_);
    cv::Mat blob = cv::dnn::blobFromImage(
        mat, 1 / this->normal[0], cv::Size(mat.cols, mat.rows),
        cv::Scalar(mean[0], mean[1], mean[2]), true, false);
    model_.setInput(blob, input_node_);
    output_tensor = model_.forward(output_node_);
    int t_size = output_tensor.size.p[0] * output_tensor.size.p[1] *
                 output_tensor.size.p[2] * output_tensor.size.p[3];
    float *out_pl = (float *)output_tensor.data;
    std::vector<float> out_vector(out_pl, out_pl + t_size);
    std::cout << "out_vector:" << out_vector.size() << std::endl;
    return out_vector;
  }
  float mean[3];
  float normal[3];

private:
  cv::dnn::Net model_;
  std::vector<int> tensor_shape_;
  std::string input_node_;
  std::string output_node_;
  cv::Mat output_tensor;
  int width_;
  int height_;
};

#endif // NCNN_MOBILEFACENET_FACE_LANDMARK_H
