# Face Series SDK Native

### 依赖

##### 3rdparty Download URL

 [12.15 更新 需要重新下载]
 http://jackyu.tpddns.cn:8004/3rdparty.zip

##### Model Download URL: 

| ARM                                                          | CPU                                                          | GPU                                                          | Public                                                       |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [arm_update_1217](http://jackyu.tpddns.cn:8005/arm/face_im_e_arm.zip) | [cpu_update_1217](http://jackyu.tpddns.cn:8005/cpu/face_im_e_cpu.zip) | [gpu_update_1217](http://jackyu.tpddns.cn:8005/gpu/face_im_e_gpu.zip) | [public_update_date_1210](http://jackyu.tpddns.cn:8005/public/face_tracker.zip) |



### 如何编译

Note: 推荐在 Linux ubuntu16.04下编译，编译之前，认为默认安装一些必要的工具链如 build-essential，cmake等

##### 1.X64 Linux 和Mac下使用CMake构建和编译方法

step.1 克隆代码到本地

​	 `git clone https://gitee.com/jackyu1127/nz_face_dev.git;`

step.2  下载所需的依赖

 	` cd  nz_face_dev` 

​	`wget  http://jackyu.tpddns.cn:8004/3rdparty.zip & unzip 3rdparty.zip -d .`

​	`wget  http://jackyu.tpddns.cn:8005/model.zip & unzip 3rdparty.zip -d .`

step.3  自行编译OpenCV C++版本目前推荐使用4.0以上版本，最新版为 4.5 ，4.5版本速度会快一点。

​	推荐的编译教程如下 https://www.cnblogs.com/laizhenghong2012/p/11329916.html （上面的编译教程只是参考，由于OpenCV ，GStreamer，FFMPEG等库受系统驱动和环境影响，因此无法打包进3rdparty 提供预编译好的包，这里麻烦各位自己编译一下。 ）

step.4 编译本项目

   在opencv编译好之后 假设opencv的目录在 `/opt/opencv` ，则编译好的build目录为 `/opt/opencv/build`

   切换到工程目录

   `cd  nz_face_dev`

   `mkdir build`

   `cmake -DOpenCV_DIR=/opt/opencv/build ../`

   如果想开启GPU 可以使用 

   `cmake -DOpenCV_DIR=/opt/opencv/build -DBUILD_CUDA=ON../`

   `make -j`

  开始编译



##### 2.在X64 Linux 下通过Android NDK交叉编译 Android ABI底层 动态库(so)的方法。

安卓编译的时候3rdparty里面的预编译的库已经准备好，直接编译即可

step.1 克隆代码到本地

​	 `git clone https://gitee.com/jackyu1127/nz_face_dev.git;`

step.2  下载所需的依赖

 	` cd  nz_face_dev` 

​	`wget  http://jackyu.tpddns.cn:8004/3rdparty.zip & unzip 3rdparty.zip -d .`

​	`wget  http://jackyu.tpddns.cn:8005/model.zip & unzip 3rdparty.zip -d .`

step.3  下载和安装NDK 

 	在linux下，android-studio 和 android sdk，不是必须的。但是进行交叉编译NDK是必须的。

​     NDK的下载地址：https://developer.android.com/ndk/downloads，我们推荐使用NDK-20b来编译（ndk-20b是我们测试过的较为稳定的版本）。

step.4 导入android环境变量 如ndk下载在 opt目录下，则

​	`export ANDROID_NDK=/opt/android-ndk-r20b`

step.5 编译android  so包

   `bash tools/compile/release_android.sh`

step.6 在编译完之后会在当目录下生成 release_android目录，里面有arm64-v8a和 armabi-v7a的分别代表了安卓64位和32位架构，像RK3399的架构是(2xA72 + 4xA53)是64位的，RK3288(4xA17)为32位架构。不过现在绝大多数arm芯片和手机都是64位架构。



### 代码设计以及规范

1.代码目前是以SDK的形式封装，每个功能类包含在一个类里面，对外用C API的形式（即动态库+一个c_api.h）头文件的形式对外提供SDK，对外使用C API好处是可以方便各个语言之间集成，如 Java 只能通过JNI来调用C接口 ，Python通过Ctypes来调用C接口。另外每个功能类的内部类接口，也在必要的时候提供。

2.目前代码的主要推理引擎是MNN：

​	Q&A：为什么使用MNN？ 

​       1.支持的后端多，不仅能在Android的ARM下推理，还能支持x86下加速，支持CUDA，OPENCL，TensorRT 等Backend。

​	   2.MNN在阿里的众多业务下，接受了大量的考验，运行稳定，出bug的概率比较低，就算出bug，也有人第一时间解决。

​	   3.目前MNN1.1的版本不管在ARM，X86，还是CUDA下都基本达到的一线水准的推理性能（在CPU上的推理成绩甚至超过了Intel自家的OpenVINO），模型转换工具支持框架多样（支持了Caffe，ONNX，Pytorch，Tensorflow在内的数百个算子，保证模型转换基本不会出问题，也不会有坑）。

​       4.MNN自带的ImageProcessing框架支持不同格式之间的ROI转换。在各类使用场景下，我们通常会接受来自不同设备的相机流格式，如标准的处理图片用的RGB888，从Android Bitmap取的RGBA，以及rtsp流解码、或者安卓相机解码之后的YUV NV21 420sp等格式。针对这些不同情况我们通常情况下，需要针对不同的情况写不同的接口，更重要的是，一般情况下相机流的视频分辨率都比较大，或前置相机带有旋转，一般在1080p左右，如果安装正常情况下，对原图进行转换的话，会多消耗一次内存拷贝时间和数据格式转换时间（普通PC这个大概在10-20ms左右），在移动端上更为明显，通常在一些单核的A53低端芯片上会占据数百毫秒的耗时（这个情况下，人脸跟踪什么的基本就用不了）。有了ImageProcessing框架，可以实现不同的输入格式到统一的输出格式在给定感兴趣的区域进行转换，这能大大降低耗时。



