### 手动编译MNN以及如何转换模型

目前给的3rdparty里面已经自带了Linux x64(amd64)，Linux x64_cuda102+cudnn7.5(amd64), Mac OS(Darwin)，Android的包， 如果想使用GPU推理，但由于不同服务器上CUDA、CUDNN版本不同，如果跟我这边的版本不一致，需要手动编译MNN。

编译方法在https://www.yuque.com/mnn/cn/build_linux上已经写的相对比较详细了，这里进一步说明一下一些编译的坑。

#### 1.编译MNN推理框架（CUDA)

##### Step.1 克隆mnn git地址

​	`git clone https://github.com/alibaba/MNN.git`

##### Step.2 编译的时候记得在查看一下CMakeLists.txt中的编译选项。

```bash
cd /path/to/MNN
./schema/generate.sh
```

##### Step.3 设置CUDA进行编译

```bash
mkdir build
cmake -D MNN_CUDA=ON ../
make -j8
```

###### 遇到的坑

1.CMake版本最好是3.10以上，太低会报错。

2.CUDA版本最好是9.2以上，测试下来这里CUDA10.2 + CUDNN7.5没有问题。

3.提示报错

`/usr/bin/ld: cannot find -lcudnn`

编译到最后的时候可能会找不到libcudnn而导致ld链接不上，可以尝试通过下面的方法来强制修正link.txt。

s.1.先找到cudnn的位置，一般情况下在 `/usr/local/cuda-版本号/lib64`

s.2.运行如下bash脚本（修改一下对应的路径）

```bash
find .|grep link.txt |xargs -L1 -I{} sed -i "s/-lcudnn/\/usr\/local\/cuda-10.2\/lib64\/libcudnn.so/g" {}
```

##### Step.3 复制到3rdparty

```bash
ldd libMNN.so 
```

查看是否连接上 libcudnn.so  

```bash
cp libMNN.so  /opt(项目目录)/nz_face_dev/3rdparty/MNN-release/linux_cuda/lib
```



#### 2.编译转换工具 

##### 1.编译

转换工具用于把不同来源的模型转换到mnn格式，编译起来相对比较容易。注意protobuf需要在3.0以上。

```bash
cd MNN/
./schema/generate.sh
mkdir build_convertor
cd build_convertor
cmake .. -DMNN_BUILD_CONVERTER=true && make -j4
```

编译完成之后会有个MNNConvert的可执行文件。

##### 2.转换模型

MNN支持转换从来源于ONNX、Tensorflow、TF-LITE、CAFFE框架。这里一般主流的使用ONNX来转换，ONNX是开放标准化的通用模型交换格式，大多数框架都支持把当前框架的模型转换到ONNX，包括了Mxnet，Pytorch，PaddlePaddle等。

我们一般mxnet使用的比较多，Mxnet的原始模型为一个用来定义模型结构的symbol.json文件，以及一个用来存放参数的params文件。

mxnet支持把mxnet模型转换到ONNX模型，可以参考来自[官方的教程](https://mxnet.apache.org/versions/1.7.0/api/python/docs/tutorials/deploy/export/onnx.html)。

下面是一个转换例子

`./MNNConvert --framework ONNX --modelFile y2.onnx --MNNModel --bizCode y2`

bizcode为模型表示符，可以随便写。