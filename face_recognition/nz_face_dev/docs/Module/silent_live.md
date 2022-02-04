## Silent Live

单目相机静默活体检测接口功能文档。

### SilentLive

单目活体检测，用于单目相机下的静默活体检测算法。

#### 1.1.1 SilentLive

##### 方法1

```C
explicit SilentLive();
```

##### 功能描述

单目静默活体SilentLive对象的构造方法。需要自行调用reset()加载模型。

##### 返回值说明

无



##### 方法2

```C
explicit SilentLive(const std::vector<std::string> &models_path);
```

##### 功能描述

单目静默活体SilentLive对象的构造方法，包含通过传入存放模型路径的vector初始化加载双目活体检测模型的功能。

##### 参数说明

| 参数        | 类型                              | 描述                           |
| ----------- | --------------------------------- | ------------------------------ |
| models_path | std::vector**<**std::string**>**& | 存放模型地址字符串的vector地址 |

##### 返回值说明

无



#### 1.1.2 Reset

##### 方法

```C
void Reset(const std::vector<std::string> &models_path);
```

##### 功能描述

通过传入存放模型路径的vector，重置单目静默活体检测模型对象。

##### 参数说明

| 参数        | 类型                              | 描述                           |
| ----------- | --------------------------------- | ------------------------------ |
| models_path | std::vector**<**std::string**>**& | 存放模型地址字符串的vector地址 |

##### 返回值说明

无



#### 1.1.3 Check

##### 方法

```C
float check(const cv::Mat &img, cv::Rect &box, bool is_crop = true);
```

##### 功能描述

传入包含人脸的图片以及人脸框的位置信息进行单目静默活体检测。

##### 参数说明

| 参数    | 类型      | 描述                             |
| ------- | --------- | -------------------------------- |
| img     | cv::Mat&  | 包含人脸的原始图像               |
| box     | cv::Rect& | 需要进行活体检测的人脸框位置信息 |
| is_crop | bool      | 是否进行裁剪，默认裁剪           |

##### 返回值说明

| 类型  | 说明                                             |
| ----- | ------------------------------------------------ |
| float | 返回单目静默活体检测的计算结果，为0到1的浮点数。 |




