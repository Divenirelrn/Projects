//
// Created by Jack on 26/12/2017.
//

#ifndef SWIFTPR_MODELLOADER_H
#define SWIFTPR_MODELLOADER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace pr {

struct ModelSize {
  int prototxt_size;
  int caffemodel_size;
};

struct Model {
  char *prototxtBuffer;
  char *caffemodelBuffer;
  ModelSize modelsize;
};

class ModelLoader {
public:
  ModelLoader(const std::string &model_path);
  ModelLoader(){};
  void Reset(const std::string &model_path);
  ~ModelLoader();
  Model *ReadModel(int idx) const;
  Model *ReadModel(const std::string &name) const;
  std::size_t Count() const;

private:
  ModelSize *modelSize;
  Model *model;
  int number_of_models;
  int magic_number;
  std::map<std::string, int> n2i_;
};
} // namespace pr

#endif // SWIFTPR_MODELLOADER_H
