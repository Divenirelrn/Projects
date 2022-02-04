#include "../src/model_infer/mnn_infer.h"

bool check_blob(const cv::Mat &blob_src, const cv::Mat &blob_rel) {
  assert(blob_src.size() == blob_rel.size());

  cv::Size ts_size = blob_src.size();
  for (int i = 0; i < size; i++) {

    CHECK();
  }
  return 0;
}
int main() {

  cv::Mat input_data = cv::imread("test.jpg");
  ModelInfer *infer = new MNNInfer("");
  infer->Init();
  infer->SetInput(input_data); // init input
  std::vector<std::string> name_lists{"conv6-3"};
  std::vector<cv::Mat> results = infer->Forward(name_lists);
  for (auto &result : results) {
    check_blob()
  }
  return 0;
}
