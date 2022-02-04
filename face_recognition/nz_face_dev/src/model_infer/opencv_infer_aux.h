#ifndef MODEL_INFER_OPENCV_INFER_H
#define MODEL_INFER_OPENCV_INFER_H

#include "model_infer.h"
#include "model_loader.h"
#include "opencv2/opencv.hpp"

class ModelInferOCV : public ModelRunner {

  ModelInferOCV(const ModelInferOCV &) = delete;
  ModelInferOCV &operator=(const ModelInferOCV &) = delete;

  ModelInferOCV();
  //~ModelInferOCV() override;

  void Init(const ModelConfig &config, ZipWrapper *wrapper) {
    config_ = config;
    if (config.model_type == "OCV") {
      if (config.model_type == "ONNX") {
        std::vector<char> file_onnx = wrapper->ReadFileBinary(config.filename);
      }
      if (config.model_type == "CAFFE") {
        std::vector<char> prototxt = wrapper->ReadFileBinary(config.filename);
        std::vector<char> caffemodel = wrapper->ReadFileBinary(
            (*config.node)["caffemodel"].As<std::string>());
        net_ = cv::dnn::readNetFromCaffe(prototxt.data(), prototxt.size(),
                                         caffemodel.data(), caffemodel.size());
      }
      if (config.model_type == "TF") {
        std::vector<char> pb = wrapper->ReadFileBinary(config.filename);
        // std::vector<char> caffemodel=
        // wrapper->ReadFileBinary((*config.node)["caffemodel"].As<std::string>());
        net_ = cv::dnn::readNetFromTensorflow(pb.data(), pb.size());
      }
    }
  }
  //    ModelType GetModelType(){
  //    }
  //
  //    std::vector<int> GetInputTensorSize(int index) {
  //    }
  //
  //    std::vector<int> GetInputTensorSize(const std::string &index_name){
  //    }
  //
  //    std::vector<int> GetOutputTensorSize(int index) {
  //
  //    }
  //    std::vector<int> GetOutputTensorSize(const std::string &index_name) {
  //
  //    }
  //
  void SetInputData(int index, const void *data) {}
  void SetInputData(const std::string &index_name, const void *data) {
    if (config_.input_data_order == "NHWC" && config_.input_size.size() == 4 &&
        config_.input_data_type == "float") {
      int n = config_.input_size[0];
      int h = config_.input_size[1];
      int w = config_.input_size[2];
      int c = config_.input_size[3];
      cv::Mat input_mat(h, w, CV_)
    }

    net_.setInput()
  }
  void RunToTargetNode(const std::vector<std::string> &node_name) {}
  void RunToTargetNode(const std::string &node_name) {}
  const void *GetOutputData(int index) {}

  const void *GetOutputData(const std::string &index_name) {}
  void ResizeInputTensor(int index, const std::vector<int> &shape) {}

private:
  cv::dnn::Net net_;
  ModelConfig config_;
};

#endif // NCNN_MOBILEFACENET_FACE_LANDMARK_H
