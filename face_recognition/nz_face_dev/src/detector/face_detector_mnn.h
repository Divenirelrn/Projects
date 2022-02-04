//
// Created by YH-Mac on 2020/12/17.
//

#ifndef LMKTRACKING_LIB_FACE_DETECTOR_H
#define LMKTRACKING_LIB_FACE_DETECTOR_H
#include <opencv2/opencv.hpp>
#include "MNN/Interpreter.hpp"

#include "MNN/MNNDefine.h"
#include "MNN/Tensor.hpp"
#include "MNN/ImageProcess.hpp"

using namespace std;

typedef struct location
{
    float _x;
    float _y;
} location;

typedef struct landmark
{
    float x1;
    float y1;
    float x2;
    float y2;
    float s;
    location point[5];
} landmark;

typedef struct rect_box
{
    float cx;
    float cy;
    float sx;
    float sy;
} rect_box;

class Retinaface
{
public:
    Retinaface() = delete;
    Retinaface(
        std::string &mnn_path, int img_size, float threshold = 0.6,
        float nms = 0.4, int num_thread = 2, bool retinaface = false);
    void face_detect(cv::Mat &bgr, std::vector<landmark> &boxes);
    ~Retinaface();

private:
    float _nms;
    float _threshold;
    float _mean_val[3];
    float _norm_vals[3] = {1.0, 1.0, 1.0};
    int _num_thread;
    int img_size;
    bool _retinaface;

    std::shared_ptr<MNN::Interpreter> ultraface_interpreter;
    MNN::Session *ultraface_session = nullptr;
    MNN::Tensor *input_tensor = nullptr;
    std::shared_ptr<MNN::CV::ImageProcess> pretreat = nullptr;
    std::vector<rect_box> anchors;

private:
    void create_anchor(std::vector<rect_box> &anchor, int w, int h);

    void create_anchor_retinaface(std::vector<rect_box> &anchor, int w, int h);

    void compose_results(
        cv::Mat img, MNN::Tensor *scores, MNN::Tensor *boxes, MNN::Tensor *landmarks, std::vector<landmark> &results);
};

#endif //LMKTRACKING_LIB_FACE_DETECTOR_H
