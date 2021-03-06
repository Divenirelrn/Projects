## Face Quality

人脸质量分析接口功能文档。

### FaceQuality

人脸质量分析对象，包含人脸质量分析和人脸姿态估计等算法。



#### 1.1.1 FaceQuality

##### 方法

```C
FaceQuality();
```

##### 功能描述

创建一个人脸质量分析对象。需要调用Reset读取模型。

##### 参数说明

无

##### 返回值说明

无



#### 1.1.2 Reset

##### 方法

```C
void Reset(const std::string &model_path);
```

##### 功能描述

重置人脸质量分析对象，需要人脸质量分析模型路径进行初始。

##### 参数说明

| 参数       | 类型    | 描述                     |
| ---------- | ------- | ------------------------ |
| model_path | string& | 传入质量分析模型模型路径 |

##### 返回值说明

无



#### 1.1.3 Extract

##### 方法

```C
void Extract(const cv::Mat &image);
```

##### 功能描述

人脸质量提取，传入包含人脸的图像，执行人脸质量的提取。

##### 参数说明

| 参数  | 类型           | 描述           |
| ----- | -------------- | -------------- |
| image | const cv::Mat& | 包含人脸的图像 |

##### 返回值说明

无



#### 1.1.4 getQuality

##### 方法

```C
std::vector<float> getQuality() const;
```

##### 功能描述

获取人脸质量提取结果。

##### 参数说明

无

##### 返回值说明

| 类型                       | 说明                               |
| -------------------------- | ---------------------------------- |
| std::vector**<**float**>** | 获取人脸质量提取结果所存放的列表。 |



#### 1.1.5 getYaw

##### 方法

```C
float getYaw() const;
```

##### 功能描述

获取当前提取人脸质量后的人脸姿态yaw转角属性。

##### 参数说明

无

##### 返回值说明

| 类型  | 说明                                          |
| ----- | --------------------------------------------- |
| float | 返回当前提取人脸质量后的人脸姿态yaw转角属性。 |



#### 1.1.6 getRoll

##### 方法

```C
float getRoll() const;
```

##### 功能描述

获取当前提取人脸质量后的人脸姿态roll转角属性。

##### 参数说明

无

##### 返回值说明

| 类型  | 说明                                           |
| ----- | ---------------------------------------------- |
| float | 返回当前提取人脸质量后的人脸姿态roll转角属性。 |



#### 1.1.7 getPitch

##### 方法

```C
float getPitch() const;
```

##### 功能描述

获取当前提取人脸质量后的人脸姿态pitch转角属性。

##### 参数说明

无

##### 返回值说明

| 类型  | 说明                                            |
| ----- | ----------------------------------------------- |
| float | 返回当前提取人脸质量后的人脸姿态pitch转角属性。 |



#### 1.1.8 getPoints

##### 方法

```C
std::vector<cv::Point2f> getPoints() const;
```

##### 功能描述

获取人脸质量提取结果中五个关键点的位置。

##### 参数说明

无

##### 返回值说明

| 类型                     | 说明                                 |
| ------------------------ | ------------------------------------ |
| std::vector<cv::Point2f> | 人脸质量提取结果中五个关键点的位置。 |

