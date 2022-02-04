#ifndef SG_C_C_API_H_
#define SG_C_C_API_H_

#include <stdint.h>
#include <vector>
#if defined(_WIN32)
#ifdef SG_BUILD_SHARED_LIB
#define SG_CAPI_EXPORT __declspec(dllexport)
#else
#define SG_CAPI_EXPORT
#endif
#else
#define SG_CAPI_EXPORT __attribute__((visibility("default")))
#endif // _WIN32

#ifdef __cplusplus
extern "C" {
#endif

typedef enum StreamFormat {
  STREAM_RGB = 0,
  STREAM_BGR = 1,
  STREAM_RGBA = 2,
  STREAM_BGRA = 3,
  STREAM_YUV_NV12 = 4,
  STREAM_YUV_NV21 = 5,
} StreamFormat;

typedef enum CameraRotation {
  CAMERA_ROTATION_0 = 0,
  CAMERA_ROTATION_90 = 1,
  CAMERA_ROTATION_180 = 2,
  CAMERA_ROTATION_270 = 3,
} CameraRotation;

typedef struct SG_CameraStream SG_CameraStream;
typedef struct SG_FaceEmbedding SG_FaceEmbedding;
typedef struct SG_FaceLivnessMonocular SG_FaceLivnessMonocular;
typedef struct SG_FaceLivnessIR SG_FaceLivnessIR;
typedef struct SG_FaceResult SG_FaceResult;
typedef struct SG_ModelLoader SG_ModelLoader;
typedef struct RetinaFacev2Mnn RetinaFacev2Mnn;
typedef struct Rect2f Rect2f;
//typedef struct cv cv;
typedef struct BoxInfo BoxInfo;

// Image and Video Stream Input Process
// Support Image(Video) Format
// ######################################
// 0.RGB888
// 1.BGR888
// 2.RBGA8888
// 3.BGRA8888
// 4.YUV_420SP_NV12
// 5.YUV_420SP_NV21
// ######################################

SG_CAPI_EXPORT extern SG_CameraStream *SG_CreateCameraStream();
SG_CAPI_EXPORT extern void
SG_ReleaseCameraStream(SG_CameraStream *camera_stream);
SG_CAPI_EXPORT extern void
SG_CameraStreamSetData(SG_CameraStream *camerat_stream, const uint8_t *data,
                       int width, int height);
SG_CAPI_EXPORT extern void
SG_CameraStreamSetRotationMode(SG_CameraStream *image_view,
                               CameraRotation mode);
SG_CAPI_EXPORT extern void
SG_CameraStreamSetStreamFormat(SG_CameraStream *image_view, StreamFormat mode);

SG_CAPI_EXPORT extern RetinaFacev2Mnn* SG_CreateDetector(const char *path_model);

SG_CAPI_EXPORT extern void 
SG_DetectorIrImage(RetinaFacev2Mnn* detector, SG_CameraStream *camera_stream, BoxInfo rect, std::vector<BoxInfo>& boxes);

typedef struct SG_LandmarkTracker SG_LandmarkTracker;

SG_CAPI_EXPORT extern SG_LandmarkTracker *
SG_CreateLandmarkTracker(const char *path_model);

SG_CAPI_EXPORT extern void
SG_LandmarkTrackerProcessFrame(SG_LandmarkTracker *tracker,
                               SG_CameraStream *camera_stream, int is_detect);

SG_CAPI_EXPORT extern void
SG_LandmarkTrackerSetSmoothRatio(SG_LandmarkTracker *model, float ratio);

SG_CAPI_EXPORT extern void SG_ReleaseLandmarkTracker(SG_LandmarkTracker *model);

SG_CAPI_EXPORT extern int
SG_LandmarkTrackerGetFaceNum(SG_LandmarkTracker *model);

SG_CAPI_EXPORT extern int
SG_LandmarkTrackerGetLandmarkNum(SG_LandmarkTracker *model);

SG_CAPI_EXPORT extern void
SG_LandmarkTrackerGetInfoLandmarks(SG_LandmarkTracker *model, int index,
                                   float *landmark);

SG_CAPI_EXPORT extern int
SG_LandmarkTrackerGetInfoTrackId(SG_LandmarkTracker *model, int index);

SG_CAPI_EXPORT extern float
SG_LandmarkTrackerGetInfoScore(SG_LandmarkTracker *model, int index);

SG_CAPI_EXPORT extern void
SG_LandmarkTrackerGetInfoEulerAngle(SG_LandmarkTracker *model, int index,
                                    float *euler_angle, int size);

SG_CAPI_EXPORT extern SG_FaceResult *
SG_LandmarkTrackerGetFaceResult(SG_LandmarkTracker *model, int index);
SG_CAPI_EXPORT extern void SG_ReleaseFaceResult(SG_FaceResult *result);

SG_CAPI_EXPORT extern SG_ModelLoader *
SG_CreateModelLoader(const char *path_model);
SG_CAPI_EXPORT extern void SG_ReleaseModelLoader(SG_ModelLoader *model_loader);

// Face Recognition Industry Related Interface

typedef enum AliveStatus {
  ALIVE = 0,
  ATTACK = 1,
  UNKOWN = 2,
} AliveStatus;

SG_CAPI_EXPORT extern SG_FaceLivnessMonocular *
SG_CreateFaceLivenessDetectorMonocular(SG_ModelLoader *model_loader);


SG_CAPI_EXPORT extern void
SG_ReleaseFaceLivenessDetectorMonocular(SG_FaceLivnessMonocular *model);

SG_CAPI_EXPORT extern float
SG_FaceLivenessDetectorMonocularCheckAlive(SG_FaceLivnessMonocular *model,
                                           SG_CameraStream *image,
                                           SG_FaceResult *result);

SG_CAPI_EXPORT extern SG_FaceLivnessIR *
SG_CreateFaceLivenessDetectorIR(SG_ModelLoader *model_loader);

SG_CAPI_EXPORT extern void
SG_ReleaseFaceLivenessDetectorIR(SG_FaceLivnessIR *model);

SG_CAPI_EXPORT extern float SG_FaceLivenessDetectorIRCheckAlive(
    SG_FaceLivnessIR *model, SG_CameraStream *image, SG_FaceResult *result);

SG_CAPI_EXPORT extern SG_FaceEmbedding *
SG_CreateFaceEmbedder(SG_ModelLoader *model_loader);

SG_CAPI_EXPORT extern void SG_ReleaseFaceEmbedder(SG_FaceEmbedding *model);

SG_CAPI_EXPORT extern void SG_FaceEmbedderGetFeature(SG_FaceEmbedding *model,
                                                     SG_CameraStream *image,
                                                     SG_FaceResult *result);

SG_CAPI_EXPORT extern int SG_FaceResultGetFeatureLength(SG_FaceResult *result);

SG_CAPI_EXPORT extern void SG_FaceEmbedderGetFeatureRaw(SG_FaceEmbedding *model,
                                                        SG_CameraStream *image,
                                                        SG_FaceResult *result);

SG_CAPI_EXPORT extern void SG_FaceEmbedderSetTag(SG_FaceEmbedding *model,
                                                 const char *tag);


SG_CAPI_EXPORT extern float *SG_FaceResultGetFeature(SG_FaceResult *result,
                                                     int *feature_len);

SG_CAPI_EXPORT extern float SG_FaceFeatureCampareNEON(const float *feature,
                                                      const float *feature_ref,
                                                      int len);

#ifdef __cplusplus
}
#endif

#endif // SG_C_C_API_H_
