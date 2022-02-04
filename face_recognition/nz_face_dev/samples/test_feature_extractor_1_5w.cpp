//
// Created by YH-Mac on 2020/9/11.
//
#include "../src/c_api/c_api.h"
#include "../src/c_api/c_api_internal.h"
#include "../src/face_quality.h"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "utils/test_folder_helper.h"
#include <sstream>
#include <string>

//#define MTCNN_BIN_1 "models/det1.bin"
//#define MTCNN_BIN_2 "models/det2.bin"
//#define MTCNN_BIN_3 "models/det3.bin"
//#define MTCNN_PARAM_1 "models/det1.param"
//#define MTCNN_PARAM_2 "models/det2.param"
//#define MTCNN_PARAM_3 "models/det3.param"
//#define Y1MODEL "models/mfngg.mnn"

using namespace std;

float feat[512] = {0.00215075,-0.0224998,-0.0900898,0.0124619,0.0488031,-0.07164,-0.0549358,-0.0227264,-0.0560694,0.0246908,0.0663162,-0.020765,0.00422989,0.0591501,0.0321226,0.0208538,-0.0525202,0.0183802,-0.00870283,-0.0551464,0.0294577,0.0365853,0.0559344,-0.00468819,0.0760844,-0.050773,-0.00984993,0.0685722,-0.102641,-0.0437314,0.0377689,0.0159945,-0.0307548,0.0268257,-0.0598445,-0.00237372,-0.0655307,0.0249165,0.0209074,-0.0111031,0.0221026,-0.0069735,-0.0483296,0.0433456,-0.0150558,-0.00887791,-0.0581413,-0.000616801,-0.0957673,0.0204459,-0.0267377,0.00620307,-0.0403411,0.0186131,-0.0612803,-0.0351227,-0.0423877,0.0361943,0.0202783,-0.0763163,0.0424587,-0.0430031,0.0478151,-0.0601236,-0.0429794,0.0347942,-0.0427502,0.0648247,0.0707841,-0.0471622,0.062915,0.0130121,0.0194984,-0.0174933,0.0583468,-0.088427,0.0513737,-0.00322072,0.0127809,-0.0505167,-0.0418643,-0.00390127,0.0423182,0.0245461,0.0679968,-0.016989,0.050402,-0.0298903,0.00881495,0.0135147,-0.0107106,-0.0392386,-0.0166781,-0.0026164,0.0383628,0.0533795,-0.0260442,-0.035093,0.00372172,-0.0215984,-0.0519044,0.0376926,-0.0141452,-0.00206562,0.0306011,0.077619,0.00994002,-0.0514016,-0.00131241,-0.0398863,0.0604067,-0.0344847,0.0827986,-0.0333531,0.107206,0.021327,0.0508612,-0.0198874,0.0320166,0.0368415,-0.0910435,0.0044496,0.0450519,0.000929522,-0.0106175,-0.0203852,-0.0183627,-0.0137131,0.0673165,0.0586799,-0.105918,0.0131749,-0.0253019,-0.0134241,-0.0682217,-0.0042467,-0.0796046,-0.0837778,0.101439,0.0550858,-0.0681136,0.0934168,-0.10155,-0.0487364,-0.019268,-0.0434162,-0.0518279,-0.0187576,0.0445753,-0.0379263,0.00393201,-0.0202468,0.00624809,0.0138378,-0.0716799,0.0535634,-0.0444304,0.0527066,-0.0624264,0.121129,-0.0385753,0.00594194,-0.00760059,0.00443398,0.039936,0.00518647,-0.0546007,-0.0212275,0.0481986,0.0397356,0.0128388,0.00590918,0.00301735,0.00953432,0.0382317,0.0055532,-0.107935,0.0307691,0.00919589,0.0432237,-0.0316766,-0.0014361,-0.0499326,0.0560289,0.0573249,-0.031804,-0.0391391,0.00577127,0.0272357,-0.0450351,-0.0130707,0.0765494,-0.0342408,0.0139901,0.0187961,-0.0246934,-0.064802,0.0756334,0.0906291,-0.00178569,-0.052414,-0.0442533,0.0280828,-0.00777106,0.097944,-0.0158726,0.0157434,0.0344127,0.0460607,-0.0262573,-0.00169386,-0.0517294,-0.0409557,-0.0423501,-0.0712363,-0.00775721,0.0412161,0.0898177,-0.034398,0.0440594,-0.0038305,-0.038698,0.0552349,0.0585652,0.107411,0.028314,-0.00366761,0.0507408,0.030831,0.0167921,-0.0149746,0.00620841,0.0630335,0.040909,-0.0866754,0.0333921,0.00182991,-0.0335515,0.00258944,-0.0291898,0.0314372,0.00998628,-0.0300222,-0.0187477,-0.0268661,-0.0223121,-0.0810681,-0.0342813,-0.034095,-0.0182077,-0.0422641,-0.0436516,0.0778899,0.0223281,0.00796931,-0.027151,-0.0354999,-0.0318409,0.0331314,0.0109133,0.0584265,0.0490483,-0.101032,0.0525323,0.0358843,-0.00488501,0.0182737,-0.00777543,0.0306077,0.0223715,-0.0397689,0.026785,0.0172466,-0.0971474,-0.0111737,0.0167186,-0.0198065,-0.0123527,0.000555701,-0.017645,0.0703235,-6.37262e-05,-0.0848227,0.00800253,-0.0397346,-0.0761437,-0.0367668,-0.0547284,0.0174786,-0.00318561,0.0851181,0.0242801,-0.0468481,0.0406311,-0.00507332,-0.0484958,0.0241497,0.0670353,0.0228722,0.0319353,0.015111,-0.0319891,-0.0660555,-0.0182887,0.132005,-0.0912866,0.00420552,-0.00207899,0.0520706,0.0492341,0.0177213,-0.0110141,0.000115958,0.0358593,0.0372533,0.0629098,0.0218004,0.0605533,-0.0336159,-0.0180133,-0.0650651,-0.0703964,0.00357896,-0.023537,-0.0707767,-0.0552668,0.0123667,-0.0768655,-0.0126596,-0.0100915,-0.0219935,-0.0236496,0.0276204,-0.0135675,-0.065556,0.0526738,-0.0789608,0.04824,-0.0751192,0.00719628,-0.0271756,-0.0300858,-0.0165926,-0.0127513,0.000590305,0.0393979,0.0429377,0.00420853,-0.016963,0.0266274,-0.0439441,0.0628153,0.0323129,-0.0622897,0.0364608,-0.000844896,-0.0212308,-0.0379105,0.0250339,-0.023017,0.0757432,0.0245427,-0.0260501,-0.0115824,-0.0359866,0.0197721,-0.0449448,0.0104926,-0.0606198,-0.0298624,-0.0313327,-0.0166004,-0.0516103,-0.0394198,-0.000297543,-0.0604773,0.0419334,0.00106766,-0.0151543,-0.0256173,0.00706457,-0.0340747,0.0428943,0.015769,0.0442992,-0.0431671,-0.0566752,-0.00945453,0.0435432,0.0601302,0.0288776,0.0624335,0.0171364,-0.0497393,-0.0319456,-0.0343791,0.0526671,0.036552,-0.0264329,-0.0227119,-0.0244807,-0.0142976,0.026749,-0.0122112,-0.0927589,-0.0188472,0.0291489,0.0278842,-0.0707906,0.080339,-0.00990343,0.0168283,-0.00406613,0.0298399,-0.0413682,-0.0682331,-0.0396417,-0.0202786,0.0379476,0.0302809,-0.0726448,-0.00336097,0.0257003,0.0257842,0.0470595,0.00380157,-0.0696817,-0.0221668,0.0234495,0.00844365,0.0384099,-0.0254033,-0.00195612,0.0147904,0.0496331,-0.0507739,0.0146748,-0.0229401,-0.00686941,-0.00879949,0.0227398,0.0448503,-0.0290537,0.0795416,-0.0370038,-0.0491951,0.0187983,0.0301896,0.0155939,0.0317591,-0.0233153,-0.0344993,-0.0119052,0.0721004,-0.0461477,0.0636543,0.0392085,-0.0148369,0.00284148,-0.0235887,-0.0346941,0.0262434,0.0260909,-0.00850115,0.0455305,-0.0117487,-0.0504467,0.000904217,-0.0212706,-0.0548614,-0.035394,-0.0270047,-0.0561064,-0.000512143,0.0108664,0.00653325,-0.038336,0.0871429,-0.0353646,-0.0268884,0.0372883,-0.0525502,-0.0094279,-0.00922785,-0.00392418,-0.103498,0.0625171,-0.0628176,-0.0553088,-0.0607024,-0.0507004,0.0775844,0.00493004,0.0702002,-0.0143553,-0.00700746,-0.05799,0.0557935,0.00516413,-0.0324979,-0.0201726,-0.0197599,0.0573189,-0.0207131,-0.0721808,-0.0479435,0.00447351,-0.0724247,0.108175,-0.0335238,-0.00951786,-0.0380472};
/**
 * argv[1] model_path
 * argv[2] images_folder_path
 * argv[3] features_result_file
 * */

void print_feature(const std::string &filename, std::vector<float> &feature) {
  int feature_len = feature.size();
  std::cout << filename << ":";
  for (int i = 0; i < feature_len; i++) {
    std::cout << feature[i] << ",";
  }
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << argv[0] << " ";
    std::cout << "models_dir pics_dir features_result_file [model]" << std::endl;
    return 0;
  }

  string images_folder_path = argv[2];
  string features_folder_path = argv[3];
  // create feature file
  std::ofstream feat_txt(features_folder_path); 

  string model_dir = argv[1];
  std::string model_face_tracker = model_dir + "/face_tracker.bin";
  std::string model_face_industry = "";
  if (argc > 4) {
    model_face_industry = argv[4];
  } else {
    model_face_industry = model_dir + "/face_im_e.bin";
  }

  SG_ModelLoader *model_loader =
      SG_CreateModelLoader(model_face_industry.c_str());
  SG_LandmarkTracker *sgLandmarkTracker =
      SG_CreateLandmarkTracker(model_face_tracker.c_str());
  SG_FaceLivnessIR *sgIrLivnessDetector =
      SG_CreateFaceLivenessDetectorIR(model_loader);
  SG_FaceLivnessMonocular *sgFaceLivnessMonocular =
      SG_CreateFaceLivenessDetectorMonocular(model_loader);
  SG_FaceEmbedding *sgFaceEmbedding =
      SG_CreateFaceEmbedder(model_loader);

  if (check_folder(images_folder_path, false)) {
    std::cout << "images_folder_path: " << images_folder_path << std::endl;
  } else {
    std::cout << "images_folder_path folder does not exist." << std::endl;
    return 1;
  }
  //check_folder(features_folder_path, true);
  // Read folder
  //string suffix = "jpg";

  string strformat[] = {"jpg", "png", "bmp", "jpeg", "JPG", "PNG", "BMP", "JPEG"};
  vector<string> suffix(strformat, strformat+8);
  
  for (int i=0; i < suffix.size(); i++) {
      cout << suffix[i] <<endl;
  }  

  vector<string> file_names;
  vector<string> path_list = readFileListSuffix_jpy(
      images_folder_path.c_str(), suffix, file_names, false);
  std::cout << "len path list" << path_list.size() << std::endl;
  // exit(0);
  int show_cout = 3;
  int show_front_index =
      path_list.size() < show_cout ? path_list.size() : show_cout;
  for (int i = 0; i < show_front_index; ++i) {
    cout << path_list[i] << endl;
  }
  cout << "....." << endl;
  cout << "Search for image file: " << path_list.size() << endl;
  // load MTCNN
  Timer timer;
  std::string txt = "";
  
  double whole_time = 0;
  double all_whole_time = 0;
  int real_det_num =0;
  for (int idx = 0; idx < path_list.size(); idx++) {
    
    //timer.tic();
    SG_FaceEmbedderSetTag(sgFaceEmbedding, file_names[idx].c_str());
    std::string path_list_idx = path_list[idx];
    cv::Mat image = cv::imread(path_list[idx]);
    std::cout << "filename:" << file_names[idx] << std::endl;
    std::cout << "path_list:" << path_list[idx] << std::endl;
    SG_CameraStream *IR_image = SG_CreateCameraStream();
    SG_CameraStreamSetRotationMode(IR_image, CAMERA_ROTATION_0);
    SG_CameraStreamSetStreamFormat(IR_image, STREAM_BGR);
    SG_CameraStreamSetData(IR_image, image.data, image.cols, image.rows);
    
    timer.tic();
    SG_LandmarkTrackerProcessFrame(sgLandmarkTracker, IR_image, true);
    int face_num = SG_LandmarkTrackerGetFaceNum(sgLandmarkTracker);
    std::cout << "face num " << face_num << std::endl;
    if (face_num > 0) {
      real_det_num += 1;
      SG_FaceResult *result =
          SG_LandmarkTrackerGetFaceResult(sgLandmarkTracker, 0);
      SG_FaceEmbedderGetFeature(sgFaceEmbedding, IR_image, result);
      //extract_time = timer.toc("process extract");
      //all_extract_time += extract_time;      

      int feature_len = 0;
      float *feature = SG_FaceResultGetFeature(result, &feature_len);
      std::cout << "feature_len:" << feature_len << std::endl;
      std::vector<float> feature_v(feature, feature + feature_len);
      
      float score = -50; 
      for (int ti = 0; ti < 100000; ti++) {
          score = SG_FaceFeatureCampareNEON(feature, feat, 512);
      }
      whole_time = timer.toc("one whole extract");
      all_whole_time += whole_time;
      //float score = SG_FaceFeatureCampareNEON(feature, feat, 512);
      std::cout << "similar socre is:" << score << " need time are:"<< whole_time <<"ms" << endl;
      //string feat_txt_path =
      //    features_folder_path + "/" + file_names[idx] + ".txt";
      //std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
      //print_feature(path_list_idx, feature_v);
      //extract_feat_to_txt(feat_txt_path, feature_v);
      

       feat_txt << file_names[idx] << ",";
       for (int i = 0; i <feature_v.size(); i++){
           feat_txt << feature_v[i] << ",";
       }
       //feat_txt << det_time << "," << extract_time <<"," << endl;
       feat_txt << endl;
       SG_ReleaseFaceResult(result);
      cout<<"average time:" << (all_whole_time+0.0001)/real_det_num << "ms"<<endl;
    }else {
      feat_txt << file_names[idx] << ",";
      feat_txt <<"none" <<endl;
    }
    
    #if 0 
    else {
      cv::Mat image_tmp;
      cv::resize(image, image_tmp, cv::Size(96, 96));
      sgFaceEmbedding->impl_quality.Extract(image_tmp);
      std::vector<cv::Point2f> pts_5 =
          sgFaceEmbedding->impl_quality.getPoints();
      for (auto &p : pts_5) {
        p.x *= 112 / 96.0f;
        p.y *= 112 / 96.0f;
      }
      cv::Mat affine = sgFaceEmbedding->impl.GetAlignmentMatrix(pts_5);
      cv::Mat aligned_112;
      cv::warpAffine(image, aligned_112, affine, cv::Size(112, 112));

      int feature_len = 0;
      SG_FaceResult result;
      std::vector<float> feature_v = sgFaceEmbedding->impl.Extract(aligned_112);
      string feat_txt_path =
          features_folder_path + "/" + file_names[idx] + ".txt";
      std::cout << path_list_idx << "->" << feat_txt_path << std::endl;
      extract_feat_to_txt(feat_txt_path, feature_v);
      print_feature(path_list_idx, feature_v);
      std::stringstream pts;
      for (auto &p : pts_5) {
        pts << p.x << " " << p.y << " ";
      }

      cv::imwrite("tmp/" + path_list[idx] + ".png", aligned_112);
      std::cout << "bbox:" << path_list[idx] << " " << 0 << " " << 0 << " "
                << 112 << " " << 112 << " " << pts.str() << std::endl;
    }
    #endif

    //        cout << feature << endl;
    //        cv::imshow("transformed", transformed);
    //        cv::waitKey(0);
  }
  feat_txt.close();
  //cout << "Feature extraction successful：" << path_list.size() << endl;
  return 0;
}
