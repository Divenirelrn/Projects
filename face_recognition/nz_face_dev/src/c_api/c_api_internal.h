#ifndef SG_C_C_API_INTERNAL_H_
#define SG_C_C_API_INTERNAL_H_

#include "c_api.h"
#include "face_info.h"
#include "face_quality.h"
#include "feature_extract.h"
#include "landmark_tracker.h"
#include "model_loader.h"
#include "silent_live.h"
#include "silent_live_ir.h"
#include <face_quality.h>

typedef struct SG_CameraStream { SG::CameraStream impl; } SG_CameraStream;

typedef struct SG_LandmarkTracker { LandmarkTracker impl; } SG_LandmarkTracker;

typedef struct SG_FaceEmbedding {

  FeatureExtract impl;
  FaceQuality impl_quality;
} SG_FaceEmbedding;

typedef struct SG_FaceLivnessMonocular {
  SilentLive impl;
} SG_FaceLivnessMonocular;

typedef struct SG_FaceLivnessIR { SilentLiveIR impl; } SG_FaceLivnessIR;

typedef struct SG_FaceResult {
  std::vector<float> feature;
  cv::Rect2f rect;
  std::vector<cv::Point2f> landmarks;
  float confidence;
  cv::Mat trans_matrix;
} SG_FaceResult;

typedef struct SG_ModelLoader { pr::ModelLoader impl; } SG_ModelLoader;

#endif // SG_C_C_API_INTERNAL_H_
