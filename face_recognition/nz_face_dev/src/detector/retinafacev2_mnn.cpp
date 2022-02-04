#include "retinafacev2_mnn.h"
#include <cmath>

RetinaFacev2Mnn::RetinaFacev2Mnn(ZipWrapper *zip_wrapper, std::string model_file) {
  
  float mean[]   = {127.5, 127.5, 127.5};
  float normal[] = {1.0/128, 1.0/128, 1.0/128};
  retinafacev2_infer.reset(new ModelInfer(zip_wrapper, model_file, 1, mean, normal));
  std::vector<std::string> output_name;

  // [ "432", "435", "448", "451", "464", "467", "480", "483", "496", "499" ]
  // stride 8
  output_name.push_back("432");
  output_name.push_back("435");
  // stride 16
  output_name.push_back("448");
  output_name.push_back("451");
  // stride 32
  output_name.push_back("464");
  output_name.push_back("467");
  // stride 64
  output_name.push_back("480");
  output_name.push_back("483");
  // stride 128
  output_name.push_back("496");
  output_name.push_back("499");

  // pad detection use 256
  retinafacev2_infer->Init("input.1", output_name, 256, 256);
  in_height       = 256;
  in_width        = 256;
  score_threshold = 0.65;
  nms_threshold   = 0.4;
}

RetinaFacev2Mnn::RetinaFacev2Mnn() {

}

int RetinaFacev2Mnn::reset(ZipWrapper *wrapper, std::string model_file) {
  float mean[]   = {127.5, 127.5, 127.5};
  float normal[] = {1.0/128, 1.0/128, 1.0/128};
  retinafacev2_infer.reset(new ModelInfer(wrapper, model_file, 1, mean, normal));
  std::vector<std::string> output_name;

  // [ "432", "435", "448", "451", "464", "467", "480", "483", "496", "499" ]
  // stride 8
  output_name.push_back("432");
  output_name.push_back("435");
  // stride 16
  output_name.push_back("448");
  output_name.push_back("451");
  // stride 32
  output_name.push_back("464");
  output_name.push_back("467");
  // stride 64
  output_name.push_back("480");
  output_name.push_back("483");
  // stride 128
  output_name.push_back("496");
  output_name.push_back("499");

  // pad detection use 256
  retinafacev2_infer->Init("input.1", output_name, 256, 256);
  in_height       = 256;
  in_width        = 256;
  score_threshold = 0.65;
  nms_threshold   = 0.4;
}

RetinaFacev2Mnn::~RetinaFacev2Mnn() {
  //
}

int RetinaFacev2Mnn::Detect(const cv::Mat &image, std::vector<BoxInfo>& boxes) {
  int real_w = image.cols;
  int real_h = image.rows;

  int top=0, bottom=0, left=0, right=0;
  if (real_h > real_w) {
    right  = real_h-real_w; 
  } else {
    bottom = real_w-real_h;
  }

  cv::Mat pad_img;
  //cv::Mat format_img;
  cv::copyMakeBorder(image, pad_img, top, bottom, left, right, BORDER_CONSTANT, Scalar(0,0,0));
  
  cv::Mat input_img = pad_img.clone();
  // have faced size is not 256 conditions, so resize it to (256,256) size
  if (pad_img.rows != in_height or pad_img.cols != in_width) {
    cv::resize(pad_img, input_img, cv::Size(in_width, in_height));
  }

  std::vector<std::vector<float>> detection_results = retinafacev2_infer->Infer(input_img);

  //printf("results_size:%d\n", detection_results.size());

  for (int i=0; i<5; i++) {
    auto cls_pred = detection_results[i*2];
    auto box_pred = detection_results[i*2+1];
    int stride    = pow(2, i+3);
    //printf("%d, stride:%d, cls_lence:%d, box_lence:%d\n", i, stride, cls_pred.size(), box_pred.size());

    int feature_h = in_height / stride;
    int feature_w = in_width  / stride;

    for (int idx = 0; idx < feature_h * feature_w; idx++) {
      //const float *scores = rows(cls_pred, idx);
      float score = cls_pred[idx];
      //printf("socre:%f\n", score);
      int row     = idx / feature_w;
      int col     = idx % feature_w;
      int cur_label = 0;
      if (score >= score_threshold) {
        float ct_x = (col + 0.0) * stride;
        float ct_y = (row + 0.0) * stride;
        std::vector<float> dis_pred;
        dis_pred.resize(4);
        dis_pred[0] = box_pred[idx*4 + 0] * stride;
        dis_pred[1] = box_pred[idx*4 + 1] * stride;
        dis_pred[2] = box_pred[idx*4 + 2] * stride;
        dis_pred[3] = box_pred[idx*4 + 3] * stride;

        float xmin = ct_x - dis_pred[0];
        float ymin = ct_y - dis_pred[1];
        float xmax = ct_x + dis_pred[2];
        float ymax = ct_y + dis_pred[3];

        boxes.push_back(BoxInfo{xmin, ymin, xmax, ymax, score, cur_label});
      }
    } 
  }

  nms(boxes, nms_threshold);
  return 0;
}

void RetinaFacev2Mnn::nms(std::vector<BoxInfo> &input_boxes, float NMS_THRESH) {
  std::sort(input_boxes.begin(), input_boxes.end(),
            [](BoxInfo a, BoxInfo b) { return a.score > b.score; });
  std::vector<float> vArea(input_boxes.size());
  for (int i = 0; i < int(input_boxes.size()); ++i) {
    vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1) *
               (input_boxes.at(i).y2 - input_boxes.at(i).y1 + 1);
  }
  for (int i = 0; i < int(input_boxes.size()); ++i) {
    // float ix1 = input_boxes[i].x1;
    // float ix2 = input_boxes[i].x2;
    // float iy1 = input_boxes[i].y1;
    // float iy2 = input_boxes[i].y2;
    for (int j = i + 1; j < int(input_boxes.size());) {
      // float jx1 = input_boxes[j].x1;
      // float jx2 = input_boxes[j].x2;
      // float jy1 = input_boxes[j].y1;
      // float jy2 = input_boxes[j].y2;
      float xx1 = (std::max)(input_boxes[i].x1, input_boxes[j].x1);
      float yy1 = (std::max)(input_boxes[i].y1, input_boxes[j].y1);
      float xx2 = (std::min)(input_boxes[i].x2, input_boxes[j].x2);
      float yy2 = (std::min)(input_boxes[i].y2, input_boxes[j].y2);
      float w = (std::max)(float(0), xx2 - xx1 + 1);
      float h = (std::max)(float(0), yy2 - yy1 + 1);
      float inter = w * h;
      float ovr = inter / (vArea[i] + vArea[j] - inter);
      // std::cout<<"SIOU:"<<w<<","<<h<<","<<xx1<<","<<xx2<<","<<yy1<<","<<yy2<<std::endl;
      // std::cout<<"IOU
      // ("<<ix1<<","<<iy1<<","<<ix2<<","<<iy2<<")("<<jx1<<","<<jy1<<","<<jx2<<","<<jy2<<")"<<i<<","<<j<<":"<<ovr<<std::endl;
      if (ovr >= NMS_THRESH) {
        input_boxes.erase(input_boxes.begin() + j);
        vArea.erase(vArea.begin() + j);
      } else {
        j++;
      }
    }
  }
}

