#include "landmark_tracker.h"
#include "context_config.h"
#include "cpu.h"
#include <utility>
#define USE_MNN_INFER

LandmarkTracker::LandmarkTracker(ZipWrapper *wrapper) { Reset(wrapper); }

LandmarkTracker::LandmarkTracker() {}

void LandmarkTracker::Reset(ZipWrapper *wrapper) {

  min_face_size_          = CONST::face_min_size;
  // here should modified for mtcnn mnn
  this->face_detector_mnn = std::make_shared<MTCNN_MNN>(wrapper);

  this->face_detector_mnn->SetMinFace(min_face_size_);

  // ncnn version
  //this->face_detector_ = std::make_shared<MTCNN>(wrapper);
  //this->face_detector_->SetMinFace(min_face_size_);

  // new test
  this->face_retinafacev2_mnn = std::make_shared<RetinaFacev2Mnn>(wrapper, "retinafacem2_256x256_sim.mnn");

  //this->face_detector_retina_ = std::make_shared<Detector>();
  //this->face_detector_retina_->Init(wrapper);

  detection_index_    = -1;
  tracking_idx_       = 0;
  detection_interval_ = CONST::detection_interval;
  LOGD("reset landmark tracker");
  float mean_vals[3]  = {0.0f, 0.0f, 0.0f};
  float norms_vals[3] = {1.0f, 1.0f, 1.0f};
  lmk_model_.reset(new ModelInfer(wrapper, "zzlmk_tiny.mnn", 1, mean_vals, norms_vals));

  std::vector<std::string> output_name;
  output_name.push_back("strided_slice");

  lmk_model_->Init("input_1", output_name, 112, 112);
}

// void LandmarkTracker::DetectFace(const cv::Mat &input, float scale) {
//    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(
//            input.data, ncnn::Mat::PIXEL_BGR2RGB, input.cols, input.rows);
//    std::vector<Bbox> finalBbox;
//    face_detector_->detectMaxFace(ncnn_img, finalBbox);
//    const int num_box = finalBbox.size();
//    std::vector<cv::Rect> bbox;
//    bbox.resize(num_box);
//    for (int i = 0; i < num_box; i++) {
//        bbox[i] = cv::Rect(finalBbox[i].x1, finalBbox[i].y1,
//                           finalBbox[i].x2 - finalBbox[i].x1 + 1,
//                           finalBbox[i].y2 - finalBbox[i].y1 + 1);
//        tracking_idx_ = tracking_idx_ + 1;
//        FaceInfo faceinfo(tracking_idx_, bbox[i], CONST::num_lmk);
//        candidate_faces_.push_back(faceinfo);
//    }
//}

void LandmarkTracker::DetectFace(const cv::Mat &input, float scale, SG::CameraStream &rgb_image, SG::CameraStream &ir_image) {
  
  std::vector<BoxInfo> mnn_boxes;
  this->face_retinafacev2_mnn->Detect(input, mnn_boxes);
  for (int i=0; i<mnn_boxes.size(); i++) {
    printf("mnn det result x1:%f, y1:%f, x2:%f, y2:%f, score:%f\n", mnn_boxes[i].x1, mnn_boxes[i].y1, mnn_boxes[i].x2, mnn_boxes[i].y2, mnn_boxes[i].score);
  }

  //std::vector<bbox> boxes;
  //face_detector_retina_->Detect(input, boxes);
  //    cv::imshow("image", input);
  //    cv::waitKey(0);
  std::vector<cv::Rect> bbox;
  //  cv::Mat tmp;
  //  input.copyTo(tmp);
  bbox.resize(mnn_boxes.size());
  for (int i = 0; i < mnn_boxes.size(); i++) {
    int x1 = static_cast<int>(mnn_boxes[i].x1);
    int y1 = static_cast<int>(mnn_boxes[i].y1);
    int x2 = static_cast<int>(mnn_boxes[i].x2);
    int y2 = static_cast<int>(mnn_boxes[i].y2);
    bbox[i] = cv::Rect(x1, y1, x2 - x1, y2 - y1);
    //printf("ncnn det result x1:%f, y1:%f, x2:%f, y2:%f, score:%f\n", boxes[i].x1, boxes[i].y1, boxes[i].x2, boxes[i].y2, boxes[i].s);
    // cv::rectangle(tmp,bbox[i],cv::Scalar(255,0,0),1);
    tracking_idx_ = tracking_idx_ + 1;
    FaceInfo faceinfo(tracking_idx_, bbox[i], scale, CONST::num_lmk);
    faceinfo.SetFaceImageData(rgb_image, ir_image);
    faceinfo.detect_bbox_ = bbox[i];
    //candidate_faces_.push_back(faceinfo);
    trackingFace.push_back(faceinfo);
  }
  //    cv::imshow("image", tmp);
  //    cv::waitKey(0);
}

static int N_st = 0;
void LandmarkTracker::SparseLandmarkPredict(const cv::Mat &raw_face_crop, std::vector<cv::Point2f> &landmarks_output, float &score, float size) {
  LOGD("ready to landmark predict");
  landmarks_output.resize(CONST::num_lmk);
  if (N_st > 500000)
    exit(0);
  N_st += 1;

  const float mean_vals[3] = {127.5f, 127.5f, 127.5f};
  const float norm_vals[3] = {1.0 / 127.5, 1.0 / 127.5, 1.0 / 127.5};

  std::vector<std::vector<float>> results = lmk_model_->Infer(raw_face_crop);
  std::vector<float> out = results[0];

  for (int j = 0; j < CONST::num_lmk; j++) {
    float x = out[j * 2 + 0] * 112;
    float y = out[j * 2 + 1] * 112;
    landmarks_output[j] = cv::Point2f(x, y);
  }
  
  //cv::Mat
  cv::Mat resize_img;
  cv::Mat format_img;
  resize(raw_face_crop, resize_img, cv::Size(24,24));
  cvtColor(resize_img, format_img, cv::COLOR_BGR2RGB);

  std::vector<std::vector<float>> rnet_out_results = face_detector_mnn->RnetInfer->Infer(format_img);
  std::vector<float> rnet_out = rnet_out_results[0];

  for (int i=0; i<rnet_out.size(); i++) {
    printf("rnetout:%f,", rnet_out[i]);
  }
  printf("\n");
  score = rnet_out[1];
  LOGD("rnet predict ok ,score: %f", score);

#if 0
  // here should modified for mtcnn mnn
  ncnn::Extractor Rnet = face_detector_->Rnet.create_extractor();
  ncnn::Mat rnet_data = ncnn::Mat::from_pixels_resize(raw_face_crop.data, ncnn::Mat::PIXEL_BGR2RGB, raw_face_crop.cols, raw_face_crop.rows, 24, 24);
  rnet_data.substract_mean_normalize(mean_vals, norm_vals);
  ncnn::set_cpu_powersave(2);
  Rnet.input("data", rnet_data);
  // Rnet.set_num_threads(1);
  ncnn::Mat out_origin;
  Rnet.extract("prob1", out_origin);

  score = out_origin[1];
  LOGD("predict ok ,score: %f, %f\n", score, out_origin[0]);
#endif
}

bool LandmarkTracker::TrackFace(SG::CameraStream &image, SG::CameraStream &ir_image, FaceInfo &face) {
  //if (face.GetConfidence() <  CONST::mtcnn_rnet_threshold) {
  //  face.DisableTracking();
  //  LOGD("flag disable TrackFace");
  //  return false;
  //}
  LOGD("start track one");
  cv::Mat affine;
  std::vector<cv::Point2f> landmark_back;
  float score;

  if (face.TrackingState() == DETECT) {
    cv::Rect rect_square = face.GetRectSquare(0.0);

    std::vector<cv::Point2f> rect_pts = Rect2Points(rect_square);
    cv::Mat rotation_mode_affine = image.GetAffineMatrix();
    assert(rotation_mode_affine.rows == 2);
    assert(rotation_mode_affine.cols == 3);
    
    cv::Mat rotation_mode_affine_inv;
    cv::invertAffineTransform(rotation_mode_affine, rotation_mode_affine_inv);
    rotation_mode_affine_inv = rotation_mode_affine;
    
    std::vector<cv::Point2f> camera_pts = ApplyTransformToPoints(rect_pts, rotation_mode_affine_inv);
    camera_pts.erase(camera_pts.end() - 1);
    std::vector<cv::Point2f> dst_pts = {{0, 0}, {112, 0}, {112, 112}};
    assert(dst_pts.size() == camera_pts.size());
    affine = cv::getAffineTransform(camera_pts, dst_pts);
    // affine = GetRectSquareAffine(rect_square);
    face.setTransMatrix(affine);
  }

  affine = face.getTransMatrix();
  cv::Mat crop;
  LOGD("get affine crop ok");
  double time1 = (double)cv::getTickCount();
  crop = image.GetAffineRGBImage(affine, 112, 112);
  cv::Mat affine_inv;
  cv::invertAffineTransform(affine, affine_inv);
  double _diff = (((double)cv::getTickCount() - time1) / cv::getTickFrequency()) * 1000;
  LOGD("affine cost %f", _diff);

  std::vector<cv::Point2f> landmark_rawout;
  std::vector<float> bbox;
  double timeStart = (double)cv::getTickCount();
  SparseLandmarkPredict(crop, landmark_rawout, score, 112);
  double nTime = (((double)cv::getTickCount() - timeStart) / cv::getTickFrequency()) * 1000;
  LOGD("sparse cost %f", nTime);

  landmark_back.resize(landmark_rawout.size());
  landmark_back = ApplyTransformToPoints(landmark_rawout, affine_inv);

  int MODE = 1;
  if (MODE > 0) {
    if (face.TrackingState() == DETECT) {
      face.ReadyTracking();
    }else if (score <  CONST::mtcnn_rnet_threshold) {
      face.DisableTracking();
      LOGD("flag disable TrackFace");
      return false;
    } else if (face.TrackingState() == READY || face.TrackingState() == TRACKING) {
      cv::Mat trans_m(2, 3, CV_64F);
      cv::Mat tmp = face.getTransMatrix();
      std::vector<cv::Point2f> inside_points = landmark_rawout;
      //      std::vector<cv::Point2f> inside_points =
      //      ApplyTransformToPoints(face.GetLanmdark(), tmp);
      cv::Mat _affine(2, 3, CV_64F);
      std::vector<cv::Point2f> mean_shape_(CONST::num_lmk);
      for (int k = 0; k < CONST::num_lmk; k++) {
        mean_shape_[k].x = CONST::mean_shape[k * 2];
        mean_shape_[k].y = CONST::mean_shape[k * 2 + 1];
      }
      SimilarityTransformEstimate(inside_points, mean_shape_, _affine);
      inside_points = ApplyTransformToPoints(inside_points, _affine);
      // RotPoints(inside_points,7.5);
      inside_points = FixPointsMeanshape(inside_points, mean_shape_);
      
      // inside_points = FixPoints(inside_points);
      //      std::vector<cv::Point2f> proxy_back(5);
      //      std::vector<cv::Point2f> proxy_inside(5);
      //      std::vector<int> index{72 ,105, 69 , 45 ,50};
      //      for(int i =0 ; i < index.size();i++) {
      //          proxy_back[i] = inside_points[index[i]];
      //          proxy_inside[i] = landmark_back[index[i]];
      //      }
      // SimilarityTransformEstimate(proxy_back, proxy_inside, trans_m);

      SimilarityTransformEstimate(landmark_back, inside_points, trans_m);
      face.setTransMatrix(trans_m);
      face.EnableTracking();
      LOGD("ready face TrackFace state %d  ", face.TrackingState());
    }
  }

  // realloc many times
  face.SetLandmarkAndBestFrameInfo(landmark_back, score, image, ir_image, true);
  //printf("setlandmakrandbestframeinfo\n");
  face.SetConfidence(score);
  face.SetContinousTimes(score);
  //face.SetBestFrameInfo(score, image, ir_image);
  face.UpdateFaceAction();
  //printf("sucess return\n");
  return true;
}

void LandmarkTracker::BlackingTrackingRegion(cv::Mat &image, cv::Rect &rect_mask) {
  int height = image.rows;
  int width = image.cols;
  cv::Rect safe_rect = ComputeSafeRect(rect_mask, height, width);
  cv::Mat subImage = image(safe_rect);
  subImage.setTo(0);
}

float LandmarkTracker::CaculateIou(FaceInfo one, FaceInfo two) {

  int xa        = max(one.bbox_.x, two.bbox_.x);
  int ya        = max(one.bbox_.y, two.bbox_.y);
  int xb        = min(one.bbox_.x + one.bbox_.width, two.bbox_.x + two.bbox_.width);
  int yb        = min(one.bbox_.y + one.bbox_.height, two.bbox_.y + two.bbox_.height);
  int onearea   = one.bbox_.width * one.bbox_.height;
  int twoarea   = two.bbox_.width * two.bbox_.height;
  int interarea = max(xb-xa, 0) * max(yb-ya, 0);
 
  return (interarea*1.0)/((onearea+twoarea-interarea)*(1.0));
}

void LandmarkTracker::MergeClosedBox(std::vector<FaceInfo>& trackingFace) {
  // merge
  int face_num    = trackingFace.size();
  float iou_score = 0;
  int one_i=0, two_i=0;
  float max_iou = 0;
  for (int i=0; i<face_num; i++) {
    for (int j=i+1; j<face_num; j++) {
      FaceInfo one = trackingFace[i];
      FaceInfo two = trackingFace[j];
      iou_score    = CaculateIou(one, two);
      if (iou_score > max_iou) {
        one_i = i;
        two_i = j;
        max_iou = iou_score;
      }
    }
  }
  if (max_iou > 0.8) {
    if (trackingFace[one_i].face_id_ < trackingFace[two_i].face_id_) {
      FaceInfo iter = trackingFace[two_i];
      if (iter.best_rgb_buffer != nullptr) {
        free(iter.best_rgb_buffer);
        iter.best_rgb_buffer = nullptr;
      }
      if (iter.best_ir_buffer != nullptr) {
        free(iter.best_ir_buffer);
        iter.best_ir_buffer = nullptr;
      }
      printf("two box has the same positon, so merge them two_i\n");
      trackingFace.erase(trackingFace.begin() + two_i);
    } else {
      FaceInfo iter = trackingFace[one_i];
      if (iter.best_rgb_buffer != nullptr) {
        free(iter.best_rgb_buffer);
        iter.best_rgb_buffer = nullptr;
      }
      if (iter.best_ir_buffer != nullptr) {
        free(iter.best_ir_buffer);
        iter.best_ir_buffer = nullptr;
      }
       printf("two box has the same positon, so merge them one_i\n");
       trackingFace.erase(trackingFace.begin() + one_i);
    }   
  }
}

void LandmarkTracker::UpdateStream(SG::CameraStream &image, SG::CameraStream &ir_image, bool is_detect) {
  detection_index_ += 1;
  if (is_detect) {
    trackingFace.clear();
  }
  if (detection_index_ % detection_interval_ == 0 || is_detect) {
//    CONST::detection_scale =  224.0f/std::max(image.GetHeight() , image.GetWidth());
//    std::cout<<"Lmk detection_scale:"<<CONST::detection_scale<<std::endl;
//    float scale = CONST::detection_scale;
    cv::Mat image_detect = image.GetPreviewImage(true);
//    cv::imwrite("image__.jpg" , image_detect);
//    cv::imshow("image" , image_detect);
//    cv::waitKey(0);
    for (auto &face : trackingFace) {
      cv::Rect m_mask_rect = face.GetRectSquare();
      std::vector<cv::Point2f> pts = Rect2Points(m_mask_rect);
      cv::Mat rotation_mode_affine = image.GetAffineMatrix();
      cv::Mat rotation_mode_affine_inv;
      cv::invertAffineTransform(rotation_mode_affine, rotation_mode_affine_inv);
      std::vector<cv::Point2f> affine_pts = ApplyTransformToPoints(pts, rotation_mode_affine_inv);
      cv::Rect mask_rect = cv::boundingRect(affine_pts);
      BlackingTrackingRegion(image_detect, mask_rect);
    }
    // do detection in thread
    LOGD("--------------detect scaled rows: %d cols: %d", image_detect.rows, image_detect.cols);
    double timeStart = (double)cv::getTickCount();
    DetectFace(image_detect, image.GetPreviewScale(), image, ir_image);
    double nTime = (((double)cv::getTickCount() - timeStart) / cv::getTickFrequency()) * 1000;
    LOGD("--------------detect track cost %f", nTime);
  }

  //LOGD("candidate_faces_:%d", candidate_faces_.size());

  //if (!candidate_faces_.empty()) {
  //  LOGD("push track face");
  //  for (int i = 0; i < candidate_faces_.size(); i++) {
  //    trackingFace.push_back(candidate_faces_[i]);
  //  }
  //  candidate_faces_.clear();
  //}
  LOGD("TrackFace lists num:%d", trackingFace.size());

  // merge same ids
  for (vector<FaceInfo>::iterator iter = trackingFace.begin(); iter != trackingFace.end();) {
    if (!TrackFace(image, ir_image, *iter)) {
      
      // free this faceinfo images data memory
      if (iter->best_rgb_buffer != nullptr) {
        free(iter->best_rgb_buffer);
        iter->best_rgb_buffer = nullptr;
      }
      if (iter->best_ir_buffer != nullptr) {
        free(iter->best_ir_buffer);
        iter->best_ir_buffer = nullptr;
      }
      printf("has not tracked, free the face image data\n");
      iter = trackingFace.erase(iter);
    } else {
      //printf("iter++\n");
      iter++;
    }
  }
  MergeClosedBox(trackingFace);
}
