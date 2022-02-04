#include "c_api.h"
#include "c_api_internal.h"
#include "face_info.h"
#include "feature_extract.h"
#include "landmark_tracker.h"
#include "silent_live.h"
#include "simd.h"
#include "zip_wrapper.h"
#include <vector>
#include <sstream>

using namespace cv;

void encryption(char *data, int length, const unsigned char *key, int key_length) {
  for (int i = 0; i < length; i++) {
    data[i] = data[i] ^ key[i % key_length];
  }
}

SG_CameraStream *SG_CreateCameraStream() { return new SG_CameraStream(); }

void SG_ReleaseCameraStream(SG_CameraStream *camera_stream) {
  delete camera_stream;
}

void SG_CameraStreamSetData(SG_CameraStream *camera_stream, const uint8_t *data, int width, int height) {
  camera_stream->impl.SetDataBuffer(data, height, width);
}

void SG_CameraStreamSetRotationMode(SG_CameraStream *camera_stream, CameraRotation mode) {
  if (mode == 1) {
    camera_stream->impl.SetRotationMode(SG::ROTATION_90);
  } else if (mode == 2) {
    camera_stream->impl.SetRotationMode(SG::ROTATION_180);
  } else if (mode == 3) {
    camera_stream->impl.SetRotationMode(SG::ROTATION_270);
  } else {
    camera_stream->impl.SetRotationMode(SG::ROTATION_0);
  }
}

void SG_CameraStreamSetStreamFormat(SG_CameraStream *camera_stream, StreamFormat mode) {
  // STREAM_RGB = 0,
  // STREAM_BGR = 1,
  // STREAM_RGBA = 2,
  // STREAM_BGRA = 3,
  // STREAM_YUV_NV12 = 4,
  // STREAM_YUV_NV21 = 5,
  if (mode == 0) {
    camera_stream->impl.SetDataFormat(SG::RGB);
  } else if (mode == 1) {
    camera_stream->impl.SetDataFormat(SG::BGR);
  } else if (mode == 2) {
    camera_stream->impl.SetDataFormat(SG::RGBA);
  } else if (mode == 3) {
    camera_stream->impl.SetDataFormat(SG::BGRA);
  } else if (mode == 4) {
    camera_stream->impl.SetDataFormat(SG::NV12);
  } else if (mode == 5) {
    camera_stream->impl.SetDataFormat(SG::NV21);
  }
}

#if 1
RetinaFacev2Mnn* SG_CreateDetector(const char *path_model) {
  ZipWrapper zipWrapper(path_model);
  RetinaFacev2Mnn* detector = new RetinaFacev2Mnn(); 
  detector->reset(&zipWrapper, "retinafacem2_256x256_sim.mnn");
  return detector;
}

void SG_DetectorIrImage(RetinaFacev2Mnn* detector, SG_CameraStream* camera_stream, BoxInfo rect, std::vector<BoxInfo>& boxes) {
  cv::Mat image_detect = camera_stream->impl.GetPreviewImage(true);
  
  int im_rows = image_detect.rows;
  int im_cols = image_detect.cols;
  // here move the rgb rect box an crop the image to detect
  int r_x1     = rect.x1;
  int r_y1     = rect.y1;
  int r_width  = rect.x2-rect.x1;
  int r_height = rect.y2-rect.y1;

  int move_x1 = r_x1 - 10;
  int move_y1 = r_y1 - 10;
  int move_w  = r_width + 20;
  int move_h  = r_height + 20;
  if (move_x1 <=0) {
    move_x1 = 1;
  }
  if (move_y1 <= 0) {
    move_y1 = 1;
  }
  if (move_w >= im_cols-move_x1) {
    move_w = im_cols-move_x1-1;
  }
  if (move_h >= im_rows-move_y1){
   move_h = im_rows-move_y1-1;
  }
 
  #if 0
  int cx = move_x1 + move_w / 2;
  int cy = move_y1 + move_h / 2;
  int R = max(move_w, move_h) / 2;
  int x1 = cx - R;
  int y1 = cy - R;
  int x2 = cx + R;
  int y2 = cy + R;
  if (x1 <= 0) { x2=x2-x1+1; x1=1; }
  if (y1 <= 0) { y2=y2-y1+1; y1=1; }

  int width = x2 - x1;
  int height = y2 - y1;
  assert(width > 0);
  assert(height > 0);
  assert(height == width);  
  #endif

  printf("sg_detectirimage:%d, %d, %d, %d, preview_size_:%d\n", move_x1, move_y1, move_w, move_h, camera_stream->impl.GetPreviewSize());
  cv::Rect box_square(move_x1, move_y1, move_w, move_h);
  cv::Mat real_det = image_detect(box_square).clone();
  
  //cv::Mat resize_im;
  float scale_det = camera_stream->impl.GetPreviewSize()*1.0/max(real_det.rows, real_det.cols);
  //cv::resize(real_det, resize_im, cv::Size(256,256));
  //cv::imwrite("real_det.jpg", resize_im);

  detector->Detect(real_det, boxes);
  for (int i=0; i<boxes.size(); i++) {
    boxes[i].x1 = boxes[i].x1 / scale_det + move_x1;
    boxes[i].y1 = boxes[i].y1 / scale_det + move_y1;
    boxes[i].x2 = boxes[i].x2 / scale_det + move_x1;
    boxes[i].y2 = boxes[i].y2 / scale_det + move_y1;
  }
  
  cv::rectangle(image_detect, cv::Rect(boxes[0].x1, boxes[0].y1, boxes[0].x2-boxes[0].x1, boxes[0].y2-boxes[0].y1), cv::Scalar(0, 0, 255), 1, 8, 0);
  cv::imwrite("real_det.jpg", image_detect);
  return;
}
#endif

SG_LandmarkTracker *SG_CreateLandmarkTracker(const char *path_model) {
  SG_LandmarkTracker *tracker = new SG_LandmarkTracker();
  ZipWrapper zipWrapper(path_model);
  tracker->impl.Reset(&zipWrapper);
  return tracker;
}

bool SortBoxSize(const FaceInfo &a , const FaceInfo &b){
    int sq_a = a.detect_bbox_.height * a.detect_bbox_.width;
    int sq_b = b.detect_bbox_.height * b.detect_bbox_.width;
    return sq_a>sq_b;
}

void SG_LandmarkTrackerProcessFrame(SG_LandmarkTracker *tracker, SG_CameraStream *camera_stream, int is_detect) {
  tracker->impl.UpdateStream(camera_stream->impl, camera_stream->impl, is_detect);
  std::sort( tracker->impl.trackingFace.begin() , tracker->impl.trackingFace.end() ,SortBoxSize );
}

void SG_LandmarkTrackerSetSmoothRatio(SG_LandmarkTracker *tracker, float ratio) {}

void SG_ReleaseLandmarkTracker(SG_LandmarkTracker *tracker) { delete tracker; }

int SG_LandmarkTrackerGetFaceNum(SG_LandmarkTracker *tracker) {
  return tracker->impl.trackingFace.size();
}

int SG_LandmarkTrackerGetLandmarkNum(SG_LandmarkTracker *tracker) {
  return 106;
}

void SG_LandmarkTrackerGetInfoLandmarks(SG_LandmarkTracker *model, int index, float *landmark) {
  std::vector<cv::Point2f> landmarks =
      model->impl.trackingFace[index].GetLanmdark();
  for (int i = 0; i < landmarks.size(); i++) {
    landmark[i * 2] = landmarks[i].x;
    landmark[i * 2 + 1] = landmarks[i].y;
  }
  // return landmarks.size();
}

int SG_LandmarkTrackerGetInfoTrackId(SG_LandmarkTracker *model, int index) {
  return model->impl.trackingFace[index].GetTrackingId();
}

float SG_LandmarkTrackerGetInfoScore(SG_LandmarkTracker *model, int index) {
  return model->impl.trackingFace[index].GetConfidence();
}

void SG_LandmarkTrackerGetInfoEulerAngle(SG_LandmarkTracker *model, int index, float *euler_angle, int size) {
  cv::Vec3f _euler_angle = model->impl.trackingFace[index].euler_angle_;
  assert(size == 3);
  euler_angle[0] = _euler_angle[0];
  euler_angle[1] = _euler_angle[1];
  euler_angle[2] = _euler_angle[2];
}

SG_FaceResult *SG_LandmarkTrackerGetFaceResult(SG_LandmarkTracker *model, int index) {
  SG_FaceResult *result = new SG_FaceResult();
  result->landmarks     = model->impl.trackingFace[index].landmark_;
  result->confidence    = model->impl.trackingFace[index].confidence_;
  result->rect          = model->impl.trackingFace[index].detect_bbox_;
  model->impl.trackingFace[index].trans_matrix_.copyTo(result->trans_matrix);
  return result;
}

void SG_ReleaseFaceResult(SG_FaceResult *result) { delete result; }

SG_ModelLoader *SG_CreateModelLoader(const char *path_model) {
  SG_ModelLoader *model_loader = new SG_ModelLoader();
  model_loader->impl.Reset(path_model);
  return model_loader;
}

void SG_ReleaseModelLoader(SG_ModelLoader *model_loader) {
  delete model_loader;
}

SG_FaceLivnessMonocular *SG_CreateFaceLivenessDetectorMonocular(SG_ModelLoader *model_loader) {
  SG_FaceLivnessMonocular *livness_monocular_detector =
      new SG_FaceLivnessMonocular();
  livness_monocular_detector->impl.Reset(model_loader->impl);
  return livness_monocular_detector;
}

void SG_ReleaseFaceLivenessDetectorMonocular(SG_FaceLivnessMonocular *model) {
  delete model;
}

float SG_FaceLivenessDetectorMonocularCheckAlive(SG_FaceLivnessMonocular *model, SG_CameraStream *image, SG_FaceResult *result) {

  cv::Mat frame = image->impl.GetScaledImage(1.0, true);
  cv::Rect rect_int(result->rect.x, result->rect.y, result->rect.width,
                    result->rect.height);
  float confidence = model->impl.check(frame, rect_int);
  return confidence;
  //    if(confidence > 0.9)
  //        return ALIVE;
  //    else if(confidence > 0.5)
  //        return UNKOWN;
  //    else
  //        return ATTACK;
}

SG_FaceLivnessIR *SG_CreateFaceLivenessDetectorIR(SG_ModelLoader *model_loader) {
  SG_FaceLivnessIR *livness_ir_detector = new SG_FaceLivnessIR();
  livness_ir_detector->impl.Reset(model_loader->impl);
  return livness_ir_detector;
}

void SG_ReleaseFaceLivenessDetectorIR(SG_FaceLivnessIR *model) { delete model; }

float SG_FaceLivenessDetectorIRCheckAlive(SG_FaceLivnessIR *model, SG_CameraStream *image, SG_FaceResult *result) {
  cv::Mat crop_image = image->impl.GetAffineRGBImage(result->trans_matrix, 112, 112);
  //cv::imwrite("crop_image.jpg", crop_image);
  //cv::Mat out_image  = ScaleAffineMatrix(result->trans_matrix, 2.0, 112, 112, 224, 224);
  //    cv::imshow("out_image", out_image);
  //    cv::waitKey(0);
  //cv::imwrite("check_alive.jpg", out_image);
  cv::Mat image_feed;
  cv::resize(crop_image, image_feed, cv::Size(96, 96));
  float confidence = model->impl.Check(image_feed);
  return confidence;
}

float SG_FaceLivenessDetectorRGBIRCheckAlive(SG_FaceLivnessIR *model, SG_CameraStream *image_IR, SG_CameraStream *image_RGB, SG_FaceResult *result) {
  cv::Mat crop_image = image_RGB->impl.GetAffineRGBImage(result->trans_matrix, 112, 112);
  cv::Mat image_feed;
  cv::resize(crop_image, image_feed, cv::Size(96, 96));
  float confidence = model->impl.Check(image_feed);
  return confidence;
}

SG_FaceEmbedding *SG_CreateFaceEmbedder(SG_ModelLoader *model_loader) {
  SG_FaceEmbedding *feature_extractor = new SG_FaceEmbedding();
  feature_extractor->impl.Reset(model_loader->impl);
  feature_extractor->impl_quality.Reset(model_loader->impl);
  return feature_extractor;
}

void SG_FaceEmbedderGetFeature(SG_FaceEmbedding *model, SG_CameraStream *image, SG_FaceResult *result) {
  cv::Rect2f detect_box_ = result->rect;
  std::cout<<"CONST::detection_scale:"<<CONST::detection_scale<<std::endl;
  std::cout<<"result->rect" << result->rect.x<< " " <<result->rect.y << " " << result->rect.width << " "<<result->rect.height<<std::endl;
  float pv_scale = image->impl.GetPreviewScale();
  detect_box_.x /= pv_scale;
  detect_box_.y /= pv_scale;
  detect_box_.width /= pv_scale;
  detect_box_.height /= pv_scale;

//  cv::Mat scaled_image = image->impl.GetScaledImage(1.0 , false);
//  cv::rectangle(scaled_image , cv::Point(detect_box_.x , detect_box_.y),cv::Point(detect_box_.x + detect_box_.width , detect_box_.y + detect_box_.height)  ,cv::Scalar(255,0,0),1);
////  cv::imshow("image" , scaled_image);
//  cv::waitKey(0);

  cv::Mat affine_crop = model->impl_quality.ComputeCropMatrix(detect_box_);
  //cv::imwrite("affine_crop.png" , affine_crop);
  cv::Mat affine_crop_inv;
  cv::invertAffineTransform(affine_crop, affine_crop_inv);
  affine_crop.convertTo(affine_crop, CV_64F);
  affine_crop_inv.convertTo(affine_crop_inv, CV_64F);
  cv::Mat crop_pre_infer = image->impl.GetAffineRGBImage(affine_crop, 96, 96);
  //cv::imwrite("crop_pre_infer.png" , crop_pre_infer);

  model->impl_quality.Extract(crop_pre_infer);
  std::vector<cv::Point2f> pts5_align_inside = model->impl_quality.getPoints();
  std::vector<cv::Point2f> pts5_align =
      ApplyTransformToPoints(pts5_align_inside, affine_crop_inv);
  std::stringstream pts;
  for (auto &p : pts5_align) {
    pts << p.x << " " << p.y << " ";
  }

  std::cout << "bbox:" << model->impl.tag << " " << detect_box_.x << " "
            << detect_box_.y << " " << detect_box_.x + detect_box_.width << " "
            << detect_box_.y + detect_box_.height << " " << pts.str()
            << std::endl;

  cv::Mat affine = model->impl.GetAlignmentMatrix(pts5_align);
  affine.convertTo(affine, CV_64F);
  // cv::Mat scaled_image = image->impl.GetScaledImage(1.0, false);
  // cv::Mat aligned_face;
  // cv::warpAffine(scaled_image,aligned_face  ,affine , cv::Size(112,112));
  cv::Mat aligned_face = image->impl.GetAffineRGBImage(affine, 112, 112);
  std::vector<float> feature = model->impl.Extract(aligned_face);
  float l2_norm  = model->impl.GetL2Norm();
  if (l2_norm < 10){
    feature.assign(feature.size(), 0.0f);
    //memset(
  }
  result->feature = feature;
}

void SG_FaceEmbedderGetFeatureRaw(SG_FaceEmbedding *model, SG_CameraStream *image, SG_FaceResult *result) {
  cv::Mat aligned_face = image->impl.GetScaledImage(1.0, false);
  // cv::resize(aligned_face ,aligned_face, cv::Size(112,112));
  std::vector<float> feature = model->impl.Extract(aligned_face);
  result->feature = feature;
}

void SG_FaceEmbedderSetTag(SG_FaceEmbedding *model, const char *tag) {
  std::string _tag(tag);
  model->impl.tag = _tag;
}

// void SG_FaceEmbedderGetFeature(SG_FaceEmbedding *model,
//                               SG_CameraStream *image,
//                               SG_FaceResult *result) {
//    std::vector<int> align_index({72 ,105, 69 , 45 ,50});
//    std::vector<cv::Point2f> align_pts;
//    for(auto index:align_index){
//        align_pts.push_back(result->landmarks[index]);
//    }
//    cv::Mat affine = model->impl.GetAlignmentMatrix(align_pts);
//    affine.convertTo(affine,CV_64F);
//    cv::Mat aligned_face = image->impl.GetAffineRGBImage(affine,112,112);
//    std::vector<float> feature = model->impl.Extract(aligned_face);
//    result->feature = feature;
//}

float *SG_FaceResultGetFeature(SG_FaceResult *result, int *feature_len) {
    std::cout<<"result->feature.size():"<<result->feature.size()<<std::endl;
  *feature_len = result->feature.size();
  return result->feature.data();
}

int SG_FaceResultGetFeatureLength(SG_FaceResult *result) {
  int feature_len = result->feature.size();
  return feature_len;
}

void SG_ReleaseFaceEmbedder(SG_FaceEmbedding *model) { delete model; }

float SG_FaceFeatureCampareNEON(const float *feature, const float *feature_ref, int len) {
  return simd_dot(feature, feature_ref, len);
}
