//
// Created by 庾金科 on 2020/9/26.
//

#include "zip_wrapper.h"
#include "Yaml.hpp"
#include <iostream>
#include <string>


//std::vector<char> ReadFile(const std::string &filename)
//{
//    std::ifstream _File(filename, std::ios::binary);
//    std::vector<char> buffer((std::istreambuf_iterator<char>(_File)), std::istreambuf_iterator<char>());
//    return buffer;
//}
//
//
//#include "zip_file.hpp"

int main()
{
    //std::vector<char> data =  ReadFile("/Users/yujinke/Downloads/artifacts_linux.zip");
    //std::vector<char> data =  ReadFile("/Users/yujinke/Downloads/artifacts_linux.zip");

    ZipWrapper file("/Users/yujinke/workspace/slmk_track/source/tools/pack/face_processor_v1.7.6.x.zip");
    //miniz_cpp::zip_file file("/Users/yujinke/workspace/slmk_track/source/tools/pack/face_processor_v1.7.6.x.zip");
    //file.printdir();
    std::string res( file.ReadFileBin("config.yaml").data());
    Yaml::Node root;
    Yaml::Parse(root, res);
    std::cout<<root["feature_extractor"]["model"].As<std::string>()<<std::endl;
//    //std::string res = file.ReadFile("config.yaml");
//    std::cout<<res<<std::endl;
//

    return 0;
}
