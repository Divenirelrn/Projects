## C API Docs  v1.0

这里维护了，对外提供的C接口的接口文档。

### 1. CameraStream

CameraStream能处理外部输入来的不同格式，不同屏幕方向的图片，视频流。由于外部调用api的时候输入可能很多情况下是多种多样的，
比如像在android平台下调用Android Camera普遍采用的是nv21格式，当使用相机竖立放置的时候，输入到算法模块的图像旋转方式其实是旋转90度的
（因为相对于设备的屏幕，设备的屏幕永远是横屏模式。）。另外更比如当处理Bitmap的时候输入的图片格式是RGB，或者RGBA的形式，
一般传统的做法我们要正对不同的输入图像格式要进行转换，但是一般转换的时候要么对整个图片进行转换，不能只转换一部分，
因为转换全图耗时在移动端非常耗时尤其是输入图片很大的情况下，有了CameraStream就能处理截取只想要转换的部分，大大提高了处理速度。

#### 1.1 SG_CreateCameraStream

##### 方法

```C
SG_CAPI_EXPORT extern void SG_CreateCameraStream(SG_CameraStream *camera_stream);
```

##### 功能描述

用来创建CameraStream句柄和对象，创建CameraStream不会分配内存，基本不耗时可以高频率使用，CameraStream中的所有输入方法是zerocopy的，只会引用内存。

##### 参数说明

无

##### 返回值说明

| 类型            | 说明                |
| --------------- | ------------------- |
| SG_CameraStream | SG_CameraStream对象 |



#### 1.2 SG_ReleaseCameraStream

##### 方法

```C
SG_CAPI_EXPORT extern void 
  SG_ReleaseCameraStream(SG_CameraStream *camera_stream);
```

##### 功能描述

销毁CameraStream句柄和对象，基本不耗时可以高频率使用。

##### 参数说明

无

##### 返回值说明

无



#### 1.3 SG_CameraStreamSetData

##### 方法

```C
SG_CAPI_EXPORT extern void
SG_CameraStreamSetData(SG_CameraStream *camera_stream, const uint8_t *data,
                       int width, int height);

```

##### 功能描述

设置Camera的数据，一般为图像数据，每一帧，或者每张图片调用的时候都要设置一次数据，CameraStream内部不会拷贝内存，所以也不会耗时。只是将指针指向data的内存区域。

##### 参数说明

| 参数          | 类型 | 描述                                 |
| ------------- | ---- | ------------------------------------ |
| camera_stream | In   | 创建好的camera_stream句柄            |
| data          | In   | 图像的原始数据，一般是以字节流的形式 |
| width         | In   | 图像宽度                             |
| height        | In   | 图像高度                             |

##### 返回值说明

无



#### 1.4 SG_CameraStreamSetRotationMode

##### 方法

```C
SG_CAPI_EXPORT extern void
SG_CameraStreamSetRotationMode(SG_CameraStream *image_view,
                               CameraRotation mode);
```

##### 功能描述

指定Camera的数据的旋转方向，如像在安卓是上自拍模式（前置摄像头portrait朝向）下，是90度朝向。

##### 枚举类型

###### CameraRotation

```C
    typedef enum CameraRotation {
      CAMERA_ROTATION_0 = 0,
      CAMERA_ROTATION_90 = 1,
      CAMERA_ROTATION_180 = 2,
      CAMERA_ROTATION_270 = 3,
    } CameraRotation;
```

| 枚举成员            | 值   | 描述                                                         |
| ------------------- | ---- | ------------------------------------------------------------ |
| CAMERA_ROTATION_0   | 0    | 0度朝向，一般是相机顺时针水平放置的朝向。也是普通图片输入的朝向。 |
| CAMERA_ROTATION_90  | 1    | 90度朝向，一般为安卓上自拍模式的朝向。                       |
| CAMERA_ROTATION_180 | 2    | 180度朝向，一般为设备导致过来，180度和270度，通常需要结合IMU（重力传感器）来设置旋转方向。 |
| CAMERA_ROTATION_270 | 3    | 270度朝向。                                                  |

##### 参数说明

| 参数       | 类型 | 描述                                              |
| ---------- | ---- | ------------------------------------------------- |
| image_view | In   | 创建好的camera_stream句柄                         |
| mode       | In   | 输入的朝向，分布是0度，90度，180度，270度四个朝向 |

##### 返回值说明

无



#### 1.5 SG_CameraStreamSetRotationMode

##### 方法

```C
SG_CAPI_EXPORT extern void
SG_CameraStreamSetStreamFormat(SG_CameraStream *image_view, StreamFormat mode);
```

##### 功能描述

指定Camera的数据的格式，如像一般情况下在安卓下默认Camera类的preview数据格式是NV21。

##### 枚举类型

###### CameraRotation

```C
    typedef enum StreamFormat {
      STREAM_RGB = 0,
      STREAM_BGR = 1,
      STREAM_RGBA = 2,
      STREAM_BGRA = 3,
      STREAM_YUV_NV12 = 4,
      STREAM_YUV_NV21 = 5,
    } StreamFormat;
```

| 枚举成员        | 值   | 描述                                                         |
| --------------- | ---- | ------------------------------------------------------------ |
| STREAM_RGB      | 0    | 普通的RGB图像，一般正常情况下的输入。                        |
| STREAM_BGR      | 1    | BGR图像，一般来源于opencv的imread，或 imdecode。             |
| STREAM_RGBA     | 2    | RGBA图像，一般来源于android的bitmap，或者windows下的bitmap。或者OpenGL的texture。 |
| STREAM_BGRA     | 3    | RGBA图像。                                                   |
| STREAM_YUV_NV12 | 4    | YUV_NV21图像，一般在android相机的输出，默认是nv21格式，还有ffmpeg的视频解码之后的原始流。 |
| STREAM_YUV_NV21 | 5    | YUV_NV12图像，一般在（部分少见设备）android相机的输出，默认是nv21格式，还有ffmpeg的视频解码之后的原始流。 |

##### 参数说明

| 参数       | 类型 | 描述                                              |
| ---------- | ---- | ------------------------------------------------- |
| image_view | In   | 创建好的camera_stream句柄                         |
| mode       | In   | 输入的朝向，分布是0度，90度，180度，270度四个朝向 |

##### 返回值说明

无

### 2. LandmarkTracker

人脸关键点-检测跟踪器，该实例用来检测和跟踪人脸，一般接收来自CameraStream的图片或者视频，平均每个人脸每帧耗时
在RK3399下的跟踪耗时<5ms，检测耗时在50ms左右，每隔20帧检测一次，每帧人脸最大移动像素间隔应当不超过25个像素，
一般用于人脸优选等场景，可输出每帧每个人脸的位置，106个人脸关键点，人脸姿态角，人脸动作等信息，不过由于进行
人脸检测必须要经过此类，所以进行人脸操作基本必须要创建此实例。

#### 2.1 SG_CreateLandmarkTracker

##### 方法

```C
SG_CAPI_EXPORT extern SG_LandmarkTracker * SG_CreateLandmarkTracker(const char *path_model);
```

##### 功能描述

用来创建SG_CreateLandmarkTracker句柄和对象，一般只创建一次，会加载模型会耗时，SG_CreateLandmarkTracker加载方式
和其他人脸模块不同，这个SG_CreateLandmarkTracker是基础模块，需要单独传入路径。

##### 参数说明

| 参数       | 类型 | 描述                             |
| ---------- | ---- | -------------------------------- |
| path_model | In   | 模型路径为face_tracker.bin的路径 |

##### 返回值说明

| 类型               | 说明                   |
| ------------------ | ---------------------- |
| SG_LandmarkTracker | SG_LandmarkTracker对象 |

#### 2.2 SG_LandmarkTrackerProcessFrame

##### 方法

```C
SG_CAPI_EXPORT extern void SG_LandmarkTrackerProcessFrame(SG_LandmarkTracker *tracker,SG_CameraStream *camera_stream, int is_detect);

```

##### 功能描述

处理一帧或者一张图片，整个流程会完成检测->关键点->人脸动作等流程，当is_detect参数为1的时候，会检测图片。
当处理视频流的时候is_detect应当设为0，则每隔20帧会检测一次人脸。

##### 参数说明

| 参数          | 类型 | 描述                                                  |
| ------------- | ---- | ----------------------------------------------------- |
| tracker       | In   | 创建tracker的对象句柄。                               |
| camera_stream | In   | 创建好的camera_stream,camera_stream中应当设置好内容。 |
| is_detect     | In   | 如果传入的camera_stream是图片则设为1，视频流的话为0.  |

##### 返回值说明

无

#### 2.3 SG_LandmarkTrackerSetSmoothRatio

##### 方法

```C
 SG_CAPI_EXPORT extern void SG_LandmarkTrackerSetSmoothRatio(SG_LandmarkTracker *model, float ratio);
```

##### 功能描述

设置人脸检测关键点tracker的平滑系数，由于视频帧之间是有微小的变化，出来的关键点可能会在暗光或者弱光情况下发生抖动，
通过设置ratio的大小可以平滑这种抖动，但是同时也会减小运动时的关键点精度，使得跟踪的关键点更顺畅，这个函数应该在创建完
SG_LandmarkTrackerProcessFrame之后使用。

##### 参数说明

| 参数  | 类型 | 描述                                        |
| ----- | ---- | ------------------------------------------- |
| model | In   | 创建tracker的对象句柄。                     |
| ratio | In   | 越小越平滑，构造Tracker的时候默认值为0.06。 |

##### 返回值说明

无

#### 2.4 SG_LandmarkTrackerGetFaceNum

##### 方法

```C
SG_CAPI_EXPORT extern int SG_LandmarkTrackerGetFaceNum(SG_LandmarkTracker *model);
```

##### 功能描述

获取跟踪中列表中的的人脸个数，一般跟着SG_LandmarkTrackerProcessFrame之后调用，获取人脸个数之后可以按照index从0开始取，
index 0号是面积最大的人脸。

##### 参数说明

| 参数    | 类型 | 描述                    |
| ------- | ---- | ----------------------- |
| tracker | In   | 创建tracker的对象句柄。 |

##### 返回值说明

| 类型               | 说明                   |
| ------------------ | ---------------------- |
| SG_LandmarkTracker | SG_LandmarkTracker对象 |

#### 2.5 SG_LandmarkTrackerGetInfoLandmarks

##### 方法

```C
SG_CAPI_EXPORT extern void SG_LandmarkTrackerGetInfoLandmarks(SG_LandmarkTracker *model, int index,float *landmark);
```

##### 功能描述

根据index获取人脸的关键点，一般跟着SG_LandmarkTrackerProcessFrame之后调用，index从0开始，0号为最大的人脸。
由于关键点长度需要通过 `SG_LandmarkTrackerGetLandmarkNum`  来获取。

##### 参数说明

| 参数     | 类型 | 描述                                                         |
| -------- | ---- | ------------------------------------------------------------ |
| model    | In   | 创建tracker的对象句柄。                                      |
| index    | In   | 人脸的索引， 0 <= index< 人脸个数。                          |
| landmark | Out  | 人脸关键点，一般为106个，输出的float长度为212，排列为x0,y0,x1,y1,x2,y2...这种形式。 |

##### 返回值说明

无

#### 2.6 SG_LandmarkTrackerGetInfoLandmarks

##### 方法

```C
SG_CAPI_EXPORT extern int SG_LandmarkTrackerGetLandmarkNum(SG_LandmarkTracker *model);
```

##### 功能描述

获取人脸关键点的长度，一般输出为212，此接口为预留接口。

##### 参数说明

| 参数  | 类型 | 描述                    |
| ----- | ---- | ----------------------- |
| model | In   | 创建tracker的对象句柄。 |

##### 返回值说明

| 类型 | 说明                            |
| ---- | ------------------------------- |
| int  | 人脸关键点的数组长度，一般为212 |

#### 

#### 2.7 SG_LandmarkTrackerGetInfoTrackId

##### 方法

```C
SG_CAPI_EXPORT extern int SG_LandmarkTrackerGetInfoTrackId(SG_LandmarkTracker *model, int index);
```

##### 功能描述

获得人脸跟踪中的TrackID，TrackID的意思是用来关联上一帧和下一帧人脸的位置。在一个人脸处于跟踪状态下，TrackID并不会改变。
人脸丢失后再进入则会再次分配TrackID。

##### 参数说明

| 参数  | 类型 | 描述                                |
| ----- | ---- | ----------------------------------- |
| model | In   | 创建tracker的对象句柄。             |
| index | In   | 人脸的索引， 0 <= index< 人脸个数。 |

##### 返回值说明

| 类型 | 说明                            |
| ---- | ------------------------------- |
| int  | 人脸关键点的数组长度，一般为212 |

#### 

#### 2.8 SG_LandmarkTrackerGetInfoEulerAngle

##### 方法

```C
SG_CAPI_EXPORT extern void SG_LandmarkTrackerGetInfoEulerAngle(SG_LandmarkTracker *model, int index,  float *euler_angle, int size);
```

##### 功能描述

获取跟踪中的人脸的姿态角，一般可用于人脸优选等场景。

##### 参数说明

| 参数        | 类型 | 描述                                                         |
| ----------- | ---- | ------------------------------------------------------------ |
| model       | In   | 创建tracker的对象句柄。                                      |
| index       | In   | 人脸的索引， 0 <= index< 人脸个数。                          |
| euler_angle | Out  | 欧拉角数组 ，v1.0版本 euler_angle需要在外面分配好数组长度为3的堆内存再传入，new float[3]。 |
| size        | Out  | euler_angle 数组长度（弃用）                                 |

##### 返回值说明

无

#### 2.9 SG_LandmarkTrackerGetInfoScore

##### 方法

```C
SG_CAPI_EXPORT extern float SG_LandmarkTrackerGetInfoScore(SG_LandmarkTracker *model, int index);
```

##### 功能描述

获得指定index，人脸的置信度。此置信度并非是质量得分，相对来讲越高，越可靠。

##### 参数说明

| 参数  | 类型 | 描述                                |
| ----- | ---- | ----------------------------------- |
| model | In   | 创建tracker的对象句柄。             |
| index | In   | 人脸的索引， 0 <= index< 人脸个数。 |

##### 返回值说明

| 类型 | 说明       |
| ---- | ---------- |
| int  | 人脸置信度 |

#### 

### 3. SG_FaceResult

目前FaceResult在LandmarkTracker内部Info基础上在外面单独封一个结构体，目的是给Face Industry模块使用，
目前v1.0版本中SG_FaceResult类仅提供少量输出接口，后期将提供更多的操作接口。

#### 3.1 SG_LandmarkTrackerGetFaceResult

##### 方法

```C
SG_CAPI_EXPORT extern SG_FaceResult *SG_LandmarkTrackerGetFaceResult(SG_LandmarkTracker *model, int index);
```

##### 功能描述

用LandmarkTracker中获取SG_FaceResult 主要给用于后面的人脸活体，人脸识别等应用

##### 参数说明

| 参数  | 类型 | 描述                                             |
| ----- | ---- | ------------------------------------------------ |
| model | In   | 创建tracker的对象句柄。                          |
| index | In   | 人脸的索引， 0 <= index< 人脸个数。0为最大人脸。 |

##### 返回值说明

| 类型          | 说明     |
| ------------- | -------- |
| SG_FaceResult | 人脸结果 |

 

#### 3.2 SG_FaceResultGetFeature

##### 方法

```C
SG_CAPI_EXPORT extern float *SG_FaceResultGetFeature(SG_FaceResult *result,int *feature_len);
```

##### 功能描述

从FaceResult中获得人脸特征向量

##### 参数说明

| 参数        | 类型 | 描述                                             |
| ----------- | ---- | ------------------------------------------------ |
| result      | In   | 人脸结果的对象                                   |
| feature_len | In   | 人脸的索引， 0 <= index< 人脸个数。0为最大人脸。 |

##### 返回值说明

| 类型    | 说明     |
| ------- | -------- |
| float[] | 人脸结果 |

#### 3.3 SG_ReleaseFaceResult

##### 方法

```C
SG_CAPI_EXPORT extern void SG_ReleaseFaceResult(SG_FaceResult *result);
```

##### 功能描述

Release释放FaceResult，在GetFaceResult由于是new出来的，所以之后要释放。

##### 参数说明

| 参数   | 类型 | 描述                    |
| ------ | ---- | ----------------------- |
| result | In   | 创建tracker的对象句柄。 |

##### 返回值说明

无

### 4. SG_ModelLoader

单独的ModelLoader用来加载Face Industry模块（活体，特征，属性等）的模型，
ModelLoader需在所有模型使用完之后才能释放。

#### 4.1 SG_CreateModelLoader

##### 方法

```C
SG_CAPI_EXPORT extern SG_ModelLoader *SG_CreateModelLoader(const char *path_model);
```

##### 功能描述

从文件创建模型对象

##### 参数说明

| 参数       | 类型 | 描述                |
| ---------- | ---- | ------------------- |
| path_model | In   | face_im_e.bin的路径 |

##### 返回值说明

| 类型           | 说明         |
| -------------- | ------------ |
| SG_ModelLoader | 模型对象句柄 |

#### 4.2 SG_ReleaseModelLoader

##### 方法

```C
SG_CAPI_EXPORT extern void SG_ReleaseModelLoader(SG_ModelLoader *model_loader);
```

##### 功能描述

销毁ModelLoader对象。

##### 参数说明

| 参数         | 类型 | 描述         |
| ------------ | ---- | ------------ |
| model_loader | In   | 模型对象句柄 |

##### 返回值说明

无

### 5. SG_FaceLivnessMonocular

人脸单目活体模块，需要传入model_loader

#### 5.1 SG_CreateFaceLivenessDetectorMonocular

##### 方法

```C
SG_CAPI_EXPORT extern SG_FaceLivnessMonocular *SG_CreateFaceLivenessDetectorMonocular(SG_ModelLoader *model_loader);
```

##### 功能描述

从文件创建模型对象

##### 参数说明

| 参数         | 类型 | 描述                                                   |
| ------------ | ---- | ------------------------------------------------------ |
| model_loader | In   | 创建好的ModelLoader，ModelLoader里面应当包含活体模型。 |

##### 返回值说明

| 类型                    | 说明     |
| ----------------------- | -------- |
| SG_FaceLivnessMonocular | 对象句柄 |

#### 5.2 SG_FaceLivenessDetectorMonocularCheckAlive

##### 方法

```C
 SG_CAPI_EXPORT extern float
 SG_FaceLivenessDetectorMonocularCheckAlive(SG_FaceLivnessMonocular *model,
                                            SG_CameraStream *image,
                                            SG_FaceResult *result);
```

##### 功能描述

检测是否是活体，返回活体置信度，一般>0.92，低要求场景下>0.89可认为是真人，一般需要和动作识别配合一起使用。

##### 参数说明

| 参数                    | 类型 | 描述                                                         |
| ----------------------- | ---- | ------------------------------------------------------------ |
| SG_FaceLivnessMonocular | In   | 创建好的ModelLoader，ModelLoader里面应当包含活体模型。       |
| SG_CameraStream         | In   | 创建好的CameraStream                                         |
| SG_FaceResult           | Out  | 从LandmarkTracker出来的SG_FaceResult，里面包含了人脸位置等信息。 |

##### 返回值说明

| 类型                    | 说明       |
| ----------------------- | ---------- |
| SG_FaceLivnessMonocular | 活体置信度 |

#### 5.3 SG_ReleaseFaceLivenessDetectorMonocular

##### 方法

```C
 SG_CAPI_EXPORT extern void SG_ReleaseFaceLivenessDetectorMonocular(SG_FaceLivnessMonocular *model);
```

##### 功能描述

销毁SG_FaceLivnessMonocular对象。

##### 参数说明

| 参数  | 类型 | 描述         |
| ----- | ---- | ------------ |
| model | In   | 模型对象句柄 |

##### 返回值说明

无

### 6. SG_FaceLivnessIR

人脸红外双目活体模块，需要传入model_loader

#### 6.1 SG_CreateFaceLivenessDetectorMonocular

##### 方法

```C
SG_CAPI_EXPORT extern SG_FaceLivnessIR *SG_CreateFaceLivenessDetectorIR(SG_ModelLoader *model_loader);
```

##### 功能描述

从文件创建模型对象

##### 参数说明

| 参数         | 类型 | 描述                                                   |
| ------------ | ---- | ------------------------------------------------------ |
| model_loader | In   | 创建好的ModelLoader，ModelLoader里面应当包含活体模型。 |

##### 返回值说明

| 类型             | 说明     |
| ---------------- | -------- |
| SG_FaceLivnessIR | 对象句柄 |

#### 6.2 SG_FaceLivenessDetectorMonocularCheckAlive

##### 方法

```C
SG_CAPI_EXPORT extern float SG_FaceLivenessDetectorIRCheckAlive(SG_FaceLivnessIR *model, SG_CameraStream *image, SG_FaceResult *result);
```

##### 功能描述

检测是否是活体，返回活体置信度，一般>0.25，低要求场景下>0.25可认为是真人。

##### 参数说明

| 参数             | 类型                                                         | 描述                                                   |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------ |
| SG_FaceLivnessIR | In                                                           | 创建好的ModelLoader，ModelLoader里面应当包含活体模型。 |
| SG_CameraStream  | In                                                           | 创建好的CameraStream                                   |
| SG_FaceResult    | 从LandmarkTracker出来的SG_FaceResult，里面包含了人脸位置等信息。 |                                                        |

##### 返回值说明

| 类型  | 说明       |
| ----- | ---------- |
| float | 活体置信度 |

#### 6.3 SG_ReleaseFaceLivenessDetectorIR

##### 方法

```C
 SG_CAPI_EXPORT extern void SG_ReleaseFaceLivenessDetectorIR(SG_FaceLivnessIR *model);
```

##### 功能描述

销毁SG_FaceLivnessIR对象。

##### 参数说明

| 参数  | 类型 | 描述         |
| ----- | ---- | ------------ |
| model | In   | 模型对象句柄 |

##### 返回值说明

无

### 7. SG_FaceEmbedding

人脸识别模块，用于抽取人脸特征，人脸比对等操作。

#### 7.1 SG_CreateFaceEmbedder

##### 方法

```C
 SG_CAPI_EXPORT extern SG_FaceEmbedding *SG_CreateFaceEmbedder(SG_ModelLoader *model_loader);
```

##### 功能描述

创建人脸映射器（人脸特征）对象句柄

##### 参数说明

| 参数         | 类型 | 描述                                                   |
| ------------ | ---- | ------------------------------------------------------ |
| model_loader | In   | 创建好的ModelLoader，ModelLoader里面应当包含特征模型。 |

##### 返回值说明

| 类型             | 说明     |
| ---------------- | -------- |
| SG_FaceEmbedding | 对象句柄 |

#### 7.2 SG_ReleaseFaceEmbedder

##### 方法

```C
SG_CAPI_EXPORT extern void SG_ReleaseFaceEmbedder(SG_FaceEmbedding *model);
```

##### 功能描述

销毁人脸映射器（人脸特征）对象句柄

##### 参数说明

| 参数  | 类型 | 描述                             |
| ----- | ---- | -------------------------------- |
| model | In   | 人脸映射器（人脸特征）对象句柄。 |

##### 返回值说明

无

#### 7.3 SG_FaceEmbedderGetFeature

##### 方法

```C
SG_CAPI_EXPORT extern void SG_FaceEmbedderGetFeature(SG_FaceEmbedding *model,
                                                     SG_CameraStream *image,
                                                     SG_FaceResult *result);
```

##### 功能描述

获取根据result中的人脸位置获取人脸特征并保存到result中，在调用完之后可调用SG_FaceResultGetFeature来获取人脸特征。

##### 参数说明

| 参数   | 类型   | 描述                                                         |
| ------ | ------ | ------------------------------------------------------------ |
| model  | In     | 人脸映射器（人脸特征）对象句柄。                             |
| image  | In     | 传入图像的CameraStream                                       |
| result | In/Out | 从LandmarkTracker返回的result，result一定要和CameraStream中的图片位置对应，即传入ProcessFrame的CameraStream |

##### 返回值说明

无

#### 7.4 SG_FaceEmbedderGetFeatureRaw

##### 方法

```C
SG_CAPI_EXPORT extern void SG_FaceEmbedderGetFeatureRaw(SG_FaceEmbedding *model,
                                                         SG_CameraStream *image,
                                                         SG_FaceResult *result);
```

##### 功能描述

调用的时候会把整个SG_CameraStream强制缩放，后直接抽取特征到result，不会取FaceResult对应位置的人脸，一般用于align好的人脸图片。

##### 参数说明

| 参数   | 类型 | 描述                                                         |
| ------ | ---- | ------------------------------------------------------------ |
| model  | In   | 人脸映射器（人脸特征）对象句柄。                             |
| image  | In   | 传入图像的CameraStream                                       |
| result | Out  | 从LandmarkTracker返回的result，result一定要和CameraStream中的图片位置对应，即传入ProcessFrame的CameraStream |

##### 返回值说明

无



#### 7.5 SG_FaceFeatureCampareNEON（SIMD)

##### 方法

```C
SG_CAPI_EXPORT extern float SG_FaceFeatureCampareNEON(const float *feature,
                                                      const float *feature_ref,
                                                      int len);
```

##### 功能描述

使用SIMD指令集加速点乘，来获得更快的比对速度。

##### 参数说明

| 参数        | 类型 | 描述                           |
| ----------- | ---- | ------------------------------ |
| feature     | In   | 特征                           |
| feature_ref | In   | 被比较的特征                   |
| result      | Out  | 两个特征的长度，长度必须相等。 |

##### 返回值说明

| 类型             | 说明     |
| ---------------- | -------- |
| SG_FaceEmbedding | 对象句柄 |

#### 


