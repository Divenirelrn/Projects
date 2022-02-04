#include <iostream>
#include <memory>
#include "model_infer/mnn_infer.h"
#include "opencv2/opencv.hpp"
#include <stdio.h>

class Proxy{
public:
    Proxy(){
        float mean[] = {0,0,0};
        float normal[] = {1,1,1};
        infer.reset(new ModelInfer(
                "/Users/yh-mac/Downloads/source/tools/pack/models/zzlmk.mnn",
                1,
                mean,
                normal
        ));

        std::vector<std::string> output_name;
        output_name.push_back("strided_slice");

        infer->Init("input_1", output_name, 112,112);
    }
    std::vector<float> Infer(const cv::Mat &input){
      std::cout<<infer->normal[0];
      //exit(0);
//        float mean[] = {0,0,0};
//        float normal[] = {1,1,1};
//        infer.reset(new ModelInfer(
//                "/Users/yh-mac/Downloads/source/tools/pack/models/zzlmk.mnn",
//                1,
//                mean,
//                normal
//        ));
        //infer->Init("input_1", "strided_slice", 112,112);
        std::vector<std::vector<float>> results  = infer->Infer(input);

        std::vector<float> out = results[0];
        return out;
    }
private:
    std::shared_ptr<ModelInfer> infer;
};

int main()
{
    cv::Mat image = cv::imread("/Users/yh-mac/Desktop/distance_2.jpg");
    cv::resize(image , image , cv::Size(112,112));
    //cv::Mat image = cv::imread("/Users/yujinke/workspace/slmk_track/source/debug.jpg");
    Proxy proxy;
    std::vector<float> out = proxy.Infer(image);
    for (int i = 0 ; i < 106 ;i++)
    {
        float x = out[i * 2 ];
        float y = out[i * 2 +1 ];
//        printf("x:%f ,y:%f \n" , x,y);
        cv::circle(
                image,
                cv::Point(static_cast<float>(x*112), static_cast<float>(y*112)),
                1,
                cv::Scalar(255, 0, 0),
                2);
    }
    cv::imshow("image", image);
    cv::waitKey(0);

    //faceDetect.detect((float*)data_new.data);
    return 0;
}

