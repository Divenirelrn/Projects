#ifndef SG_C_C_API_INTERNAL_H_
#define SG_C_C_API_INTERNAL_H_

#include "c_api.h"
#include "face_info.h"
#include "landmark_tracker.h"

typedef struct SG_CameraStream { SG::CameraStream impl; } SG_CameraView;

typedef struct SG_LandmarkTracker { LandmarkTracker impl; } SG_LandmarkTracker;

#endif // SG_C_C_API_INTERNAL_H_
