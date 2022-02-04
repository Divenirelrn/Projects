//
// Created by YH-Mac on 2020/9/10.
//
#include <iostream>
#include <memory>
#include "model_infer/mnn_infer.h"
#include "opencv2/opencv.hpp"
#include "../src/feature_extract.h"
#include "../src/silent_live.h"

class Proxy {
public:
    Proxy() {
        float mean[] = {127.5, 127.5, 127.5f};
        float normal[] = {0.0078125, 0.0078125, 0.0078125};
        infer.reset(new ModelInfer(
                "/Users/yh-mac/Downloads/source/tools/pack/mfngg.mnn",
                1,
                mean,
                normal
        ));

        std::vector<std::string> output_name;
        output_name.push_back("fc1_scale");

        infer->Init("data", output_name, 112, 112);
    }

    std::vector<float> Infer(const cv::Mat &input) {

        //std::vector<float> out = infer->Infer(input);
        std::vector<std::vector<float>> results  = infer->Infer(input);
        std::vector<float> out = results[0];

        return out;
    }

private:
    std::shared_ptr<ModelInfer> infer;
};

int main(int argc, char **argv) {
    cv::Mat image = cv::imread("/Users/yh-mac/Desktop/distance_2.jpg");
    cv::resize(image, image, cv::Size(112, 112));
//    Proxy proxy;
//    std::vector<float> out = proxy.Infer(image);
    FeatureExtract featureExtract("/Users/yh-mac/Downloads/source/tools/pack/mfngg.mnn");
    std::vector<float> out = featureExtract.Extract(image);
    std::cout << out.size() << std::endl;
//    for (int i = 0; i < out.size(); ++i) {
//        std::cout << out[i] << std::endl;
//    }

    cv::Mat img_src27 = cv::imread("/Users/yh-mac/fromgithub/Silent-Face-Anti-Spoofing/images/sample/fake_img1_27.jpg");
    std::vector<std::string> models_path{
            "/Users/yh-mac/Downloads/source/tools/pack/sfas_27.mnn",
            "/Users/yh-mac/Downloads/source/tools/pack/sfas_40.mnn"
    };
    SilentLive silentLive(models_path);
    cv::Rect r(1, 11,1,1);
    float res27 = silentLive.check(img_src27, r, false);
    cout << res27 << endl;
    return 0;
}
