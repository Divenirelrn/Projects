## CameraStream

相机流处理对象。

### 1.1 CameraStream

CameraStream能处理外部输入来的不同格式，不同屏幕方向的图片，视频流。



#### 1.1.1 CameraStream

##### 方法

```C
CameraStream();
```

##### 功能描述

CameraStream类别的构造方法，用于创建一个CameraStream对象并对类中的配置进行初始化。

##### 参数说明

无

##### 返回值说明

无



#### 1.1.2 SetDataBuffer

##### 方法

```C
void SetDataBuffer(const uint8_t *data_buffer, int height, int width);
```

##### 功能描述

设置Camera的图像数据，可以是相机的一帧或者一张图像。每次调用时都需要把数据传入，该方法不会拷贝内存，只是指针指向data_buffer。

##### 参数说明

| 参数        | 类型     | 描述                                 |
| ----------- | -------- | ------------------------------------ |
| data_buffer | uint8_t* | 图像的原始数据，一般是以字节流的形式 |
| width       | int      | 图像宽度                             |
| height      | int      | 图像高度                             |

##### 返回值说明

无



#### 1.1.3 SetPreviewSize

##### 方法

```C
void SetPreviewSize(const int size)
```

##### 功能描述

设置图像缩放图的尺寸，会自动按比例固定输入尺寸作为最大边进行图像的缩放。

##### 参数说明

| 参数        | 类型     | 描述                                 |
| ----------- | -------- | ------------------------------------ |
| data_buffer | uint8_t* | 图像的原始数据，一般是以字节流的形式 |

##### 返回值说明

无



#### 1.1.4 SetRotationMode

##### 方法

```C
void SetRotationMode(ROTATION_MODE mode);
```

##### 功能描述

设置Camera的数据的旋转方向，如像在安卓是上自拍模式（前置摄像头portrait朝向）下，是90度朝向。

##### 枚举类型

```c
enum ROTATION_MODE {
  ROTATION_0 = 0,
  ROTATION_90 = 1,
  ROTATION_180 = 2,
  ROTATION_270 = 3
};

```

| 枚举成员     | 值   | 描述                                                         |
| ------------ | ---- | ------------------------------------------------------------ |
| ROTATION_0   | 0    | 0度朝向，一般是相机顺时针水平放置的朝向。也是普通图片输入的朝向。 |
| ROTATION_90  | 1    | 90度朝向，一般为安卓上自拍模式的朝向。                       |
| ROTATION_180 | 2    | 180度朝向，一般为设备导致过来，180度和270度，通常需要结合IMU（重力传感器）来设置旋转方向。 |
| ROTATION_270 | 3    | 270度朝向。                                                  |

##### 参数说明

| 参数 | 类型 | 描述                                              |
| ---- | ---- | ------------------------------------------------- |
| mode | int  | 输入的朝向，分布是0度，90度，180度，270度四个朝向 |

##### 返回值说明

无



#### 1.1.5 SetDataFormat

##### 方法

```C
void SetDataFormat(ROTATION_MODE mode);
```

##### 功能描述

指定Camera的数据的格式，如像一般情况下在安卓下默认Camera类的preview数据格式是NV21。

##### 枚举类型

```c
enum DATA_FORMAT{
    NV21 = 0,
    NV12 = 1,
    RGBA = 2,
    RGB = 3,
    BGR = 4,
    BGRA = 5
};

```

| 枚举成员 | 值   | 描述                                                         |
| -------- | ---- | ------------------------------------------------------------ |
| NV21     | 0    | YUV_NV21图像，一般在android相机的输出，默认是nv21格式， 还有ffmpeg的视频解码之后的原始流。 |
| NV12     | 1    | YUV_NV12图像，一般在(部分少⻅设备)android相机的输出， 默认是nv21格式，还有ffmpeg的视频解码之后的原始流。 |
| RGBA     | 2    | RGBA图像，一般来源于android的bitmap，或者windows下的 bitmap。或者OpenGL的texture。 |
| RGB      | 3    | 普通的RGB图像，一般正常情况下的输入。                        |
| BGR      | 4    | BGR图像，一般来源于opencv的imread，或 imdecode。             |
| BGRA     | 5    | RGBA图像。                                                   |

##### 参数说明

| 参数       | 类型   | 描述           |
| ---------- | ------ | -------------- |
| model_path | string | 传入图像的格式 |

##### 返回值说明

无



#### 1.1.6 SetDataFormat

##### 方法

```C
cv::Mat GetAffineRGBImage(const cv::Mat &affine_matrix, const int width_out,
                              const int height_out);
```

##### 功能描述

传入变换矩阵，获取当前图像经过仿射变换后的图像。

##### 数说明

| 参数          | 类型     | 描述                     |
| ------------- | -------- | ------------------------ |
| affine_matrix | cv::Mat& | 传入一个变换矩阵矩阵引用 |
| width_out     | int      | 输出图像的宽度尺寸       |
| height_out    | int      | 输出图像的高度尺寸       |

##### 返回值说明

| 类型    | 说明                 |
| ------- | -------------------- |
| cv::Mat | 返回经过变换后的图像 |



#### 1.1.7 GetPreviewImage

##### 方法

```C
cv::Mat GetPreviewImage(bool with_rotation);
```

##### 功能描述

获取经过缩放后的图像。

##### 参数说明

| 参数          | 类型 | 描述             |
| ------------- | ---- | ---------------- |
| with_rotation | bool | 是否保持旋转角度 |

##### 返回值说明

| 类型    | 说明             |
| ------- | ---------------- |
| cv::Mat | 得到缩放后的图像 |



#### 1.1.8 GetPreviewScale

##### 方法

```C
float GetPreviewScale();
```

##### 功能描述

获取当前图像缩放比。

##### 参数说明

无

##### 返回值说明

| 类型  | 说明               |
| ----- | ------------------ |
| float | 获取图像当前缩放比 |



#### 1.1.9 GetScaledImage

##### 方法

```C
cv::Mat GetScaledImage(const float scale, bool with_rotation);
```

##### 功能描述

传入缩放比获取当前图像缩放。

##### 参数说明

| 参数          | 类型  | 描述             |
| ------------- | ----- | ---------------- |
| scale         | float | 图像缩放比例     |
| with_rotation | bool  | 是否保持旋转角度 |

##### 返回值说明

| 类型    | 说明                 |
| ------- | -------------------- |
| cv::Mat | 返回经过变换后的图像 |



#### 1.1.10 GetAffineMatrix

##### 方法

```C
cv::Mat GetAffineMatrix();
```

##### 功能描述

获取当前仿射变换的变换矩阵。

##### 参数说明

无

##### 返回值说明

| 类型    | 说明                     |
| ------- | ------------------------ |
| cv::Mat | 返回经过计算后的变换矩阵 |



#### 1.1.11 GetHeight

##### 方法

```C
int GetHeight();
```

##### 功能描述

获取当前图像的高度。

##### 参数说明

无

##### 返回值说明

| 类型 | 说明               |
| ---- | ------------------ |
| int  | 获取当前图像的高度 |



#### 1.1.12 GetWidth

##### 方法

```C
int GetWidth();
```

##### 功能描述

获取当前图像的宽度。

##### 参数说明

无

##### 返回值说明

| 类型 | 说明               |
| ---- | ------------------ |
| int  | 获取当前图像的宽度 |