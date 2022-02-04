#include "c_api/c_api.h"
#include "opencv2/opencv.hpp"
#include "utils/test_helper.h"
#include <string>

int main(int argc, char **argv) {

  std::string model_face_industry =
      "/Users/yujinke/workspace/slmk_track/source/tools/pack/face_im_e.bin";
  std::string model_face_tracker =
      "/Users/yujinke/workspace/slmk_track/source/tools/pack/face_tracker.zip";
  // std::string model_face_tracker =
  // "/Users/yujinke/workspace/slmk_track/source/tools/pack/face_lmktracker_e.bin";

  SG_ModelLoader *model_loader =
      SG_CreateModelLoader(model_face_industry.c_str());

  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_face_tracker.c_str());
  SG_FaceLivnessIR *sgIrLivnessDetector =
      SG_CreateFaceLivenessDetectorIR(model_loader);
  SG_FaceLivnessMonocular *sgFaceLivnessMonocular =
      SG_CreateFaceLivenessDetectorMonocular(model_loader);
  SG_FaceEmbedding *sgFaceEmbedding = SG_CreateFaceEmbedder(model_loader);
  // cv::Mat image =
  // cv::imread("/Users/yujinke/Downloads/skgg/data/padData/3_liveness/3_liveness2_pad/rgb/00031.jpg");
  // cv::Mat nir_image =
  // cv::imread("/Users/yujinke/Downloads/skgg/data/padData/3_liveness/3_liveness2_pad/nir/00031.jpg");
  cv::Mat image = cv::imread("/Users/yujinke/Downloads/skgg/data/padData/"
                             "3_liveness/3_liveness2_pad/rgb/00037.jpg");
  cv::Mat nir_image = cv::imread("/Users/yujinke/Downloads/skgg/data/padData/"
                                 "3_liveness/3_liveness2_pad/nir/00037.jpg");
  //  cv::Mat image =
  //  cv::imread("/Users/yujinke/Downloads/skgg/data/padData/3_liveness/3_liveness2_pad/rgb/000040.jpg");
  //  cv::Mat nir_image =
  //  cv::imread("/Users/yujinke/Downloads/skgg/data/padData/3_liveness/3_liveness2_pad/nir/000040.jpg");

  SG_CameraStream *RGB_image = SG_CreateCameraStream();
  SG_CameraStreamSetRotationMode(RGB_image, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(RGB_image, STREAM_BGR);
  SG_CameraStreamSetData(RGB_image, image.data, image.cols, image.rows);
  SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, RGB_image, true);
  int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
  SG_FaceResult *rgb_result;
  if (face_num == 1) {
    rgb_result = SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, 0);
    AliveStatus status = SG_FaceLivenessDetectorMonocularCheckAlive(
        sgFaceLivnessMonocular, RGB_image, rgb_result);
    std::cout << "DetectorMonocular:" << status << std::endl;
  }

  SG_CameraStream *IR_image = SG_CreateCameraStream();
  SG_CameraStreamSetRotationMode(IR_image, CAMERA_ROTATION_0);
  SG_CameraStreamSetStreamFormat(IR_image, STREAM_BGR);
  SG_CameraStreamSetData(IR_image, nir_image.data, nir_image.cols,
                         nir_image.rows);
  SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, IR_image, true);
  face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
  if (face_num == 1) {
    SG_FaceResult *result =
        SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, 0);
    AliveStatus status = SG_FaceLivenessDetectorIRCheckAlive(
        sgIrLivnessDetector, IR_image, result);
    SG_FaceEmbedderGetFeature(sgFaceEmbedding, RGB_image, rgb_result);
    int feature_len = 0;
    float *feature = SG_FaceResultGetFeature(result, &feature_len);
    std::cout << "feature_len:" << feature_len << std::endl;

    // float *feature = SG_FaceResultGetFeature(sgFaceEmbedding,&feature_len);
    // SG_FaceResultGetFeatureLength(result);
    std::cout << "DetectIR:" << status << std::endl;
    SG_ReleaseFaceResult(result);
  }

  SG_ReleaseFaceResult(rgb_result);
  SG_ReleaseFaceEmbedder(sgFaceEmbedding);
  SG_ReleaseCameraStream(RGB_image);
  SG_ReleaseCameraStream(IR_image);
  SG_ReleaseFaceLivenessDetectorIR(sgIrLivnessDetector);
  SG_ReleaseFaceLivenessDetectorMonocular(sgFaceLivnessMonocular);
}
