#ifndef LMKTRACKING_LIB_MODEL_INFER_H
#define LMKTRACKING_LIB_MODEL_INFER_H

#include "Yaml.hpp"
#include "zip_wrapper.h"
#include <vector>

enum ModelType {
  MODEL_UNKNOWN = 0,
  MODEL_MNN = 1,
  MODEL_OPENCV = 2,
  MODEL_NCNN = 3,
};

struct ModelConfig {
  ModelType type = MODEL_OPENCV;
  int threads = 1;
  std::string version;
  std::string name;
  std::string filename;
  std::vector<float> norm_vals;
  std::vector<float> mean_vals;
  std::vector<int> input_size;
  std::vector<int> output_size;
  std::string input_data_order;
  std::string input_data_type;
  std::string infer_engine;
  std::string infer_backend;
  std::string infer_device;
  std::string model_type;
  Yaml::Node *node;
  void FromYmalNode(Yaml::Node &node_value) {
    name = node_value["name"].As<std::string>();
    filename = node_value["file_name"].As<std::string>();
    version = node_value["version"].As<std::string>();
    model_type = node_value["model_type"].As<std::string>();
    infer_engine = node_value["infer_engine"].As<std::string>();
    infer_device = node_value["infer_deivce"].As<std::string>();
    infer_backend = node_value["infer_backend"].As<std::string>();
    input_data_order = node_value["input_data_order"].As<std::string>();
    input_data_type = node_value["input_data_type"].As<std::string>();
    input_size = node_value["input_size"].As<std::vector<int>>();
    output_size = node_value["output_size"].As<std::vector<int>>();
    norm_vals = node_value["norm_vals"].As<std::vector<float>>();
    mean_vals = node_value["mean_vals"].As<std::vector<float>>();
    node = &node_value;
  }

  std::string ToInfo() const {
    std::stringstream ss;
    return ss.str();
  }
};

class ModelRunner {
public:
  virtual ~ModelRunner() {}
  virtual int Init(const ModelConfig &config, ZipWrapper *wrapper) = 0;
  virtual ModelType GetModelType() const = 0;
  virtual std::vector<int> GetInputTensorSize(int index) const = 0;
  virtual std::vector<int>
  GetInputTensorSize(const std::string &index_name) const = 0;
  virtual std::vector<int> GetOutputTensorSize(int index) const = 0;
  virtual std::vector<int>
  GetOutputTensorSize(const std::string &index_name) const = 0;
  virtual void SetInputData(int index, const void *data) = 0;
  virtual void SetInputData(const std::string &index_name,
                            const void *data) = 0;
  virtual void RunToTargetNode(const std::vector<std::string> &node_name) = 0;
  virtual void RunToTargetNode(const std::string &node_name) = 0;
  virtual const void *GetOutputData(int index) = 0;
  virtual const void *GetOutputData(const std::string &index_name) = 0;
  virtual void ResizeInputTensor(int index, const std::vector<int> &shape) = 0;
};
// using ModelPtr = std::shared_ptr<Model>;

#endif // LMKTRACKING_LIB_MODEL_INFER_H
