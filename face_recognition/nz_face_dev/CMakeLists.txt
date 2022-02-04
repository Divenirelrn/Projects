cmake_minimum_required(VERSION 3.4.1) 
project(lmktracking-lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(BUILD_SHARED       "Build shared libs" ON)
option(BUILD_TEST         "Build tests "      ON)
option(BUILD_WITH_MP      "Build openmp "     OFF)
option(BUILD_TRACK_TOOL   "Build track tool " ON)
option(BUILD_CHINA_MOBILE "Build chinamobile tool " ON)
option(BUILD_WITH_ZIP     "Build Zip File Loader" ON)
option(BUILD_WITH_YMAL    "Use Yaml" ON)
option(BUILD_CUDA         "USE CUDA" OFF)


find_package(OpenCV REQUIRED )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x")
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/miniz)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/mini-yaml/yaml)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g0 -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g0 -s")


if(APPLE)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MNN-release/darwin/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ncnn/darwin/include/ncnn)
  link_directories(3rdparty/MNN-release/darwin/lib)
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ncnn/darwin/lib)
elseif(ANDROID)
  message("release android")
  include_directories(${NCNN_DIR}/../include/ncnn)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MNN-release/darwin/include)
  link_directories(${NCNN_DIR})
  link_directories(${MNN_DIR})
else()
  message("linux")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ncnn/linux/include/ncnn)
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ncnn/linux/lib)
  if(BUILD_CUDA)
      message("USE CUDA")
      add_definitions("-DFORWARD_CUDA")   #add definitions
      include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MNN-release/linux_cuda/include)
      link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MNN-release/linux_cuda/lib)
  else()
      include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MNN-release/darwin/include)
      link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MNN-release/linux/lib)
  endif()
endif()


# set(build_src src/mtcnn.cpp src/model_loader.cpp src/landmark_tracker.cpp src/c_api/c_api.cc)
#set(build_src src/mtcnn.cpp src/model_loader.cpp src/landmark_tracker.cpp src/c_api/c_api.cc src/feature_extract.cpp src/feature_extract.h samples/utils/test_folder_helper.h src/face_preprocess.h src/silent_live.cpp src/silent_live.h)
set(build_src   src/detector/retinafacev2_mnn.cpp
                src/mtcnn.cpp
                src/mtcnn_mnn.cpp
                src/model_loader.cpp
                src/landmark_tracker.cpp
                src/c_api/c_api.cc
                src/feature_extract.cpp
                src/silent_live.cpp
                src/face_attribute.cpp
                src/face_quality.cpp
                src/silent_live_ir.cpp
                3rdparty/mini-yaml/yaml/Yaml.cpp
                src/zip_wrapper.cpp
                src/detector/face_detector_retina.cpp
                src/detector/face_detector_mnn.h
                src/detector/face_detector_mnn.cpp )


#set(build_src src/mtcnn.cpp src/model_loader.cpp src/landmark_tracker.cpp src/c_api/c_api.cc src/feature_extract.cpp src/silent_live.cpp src/face_attribute.cpp src/face_quality.cpp src/silent_live_ir.cpp)

if(BUILD_WITH_MP)
  if(ANDROID OR IOS)
    message(${OpenCV_DIR})
    message(${NCNN_DIR})
    message(${MNN_DIR})
    find_package(OpenMP)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endif()


if(BUILD_SHARED)
  add_library(lmktrack SHARED ${build_src})
  if(ANDROID)
    set_target_properties(lmktrack PROPERTIES COMPILE_FLAGS -mfpu=neon)
    target_link_libraries(lmktrack ${OpenCV_LIBS} ncnn log -Wl,--whole-archive MNN -Wl,--no-whole-archive )
  elseif(APPLE)
    target_link_libraries(lmktrack ${OpenCV_LIBS} ncnn MNN)
  else()
    # Linux
    target_link_libraries(lmktrack ${OpenCV_LIBS} ncnn -Wl,--whole-archive MNN -Wl,--no-whole-archive )
  endif()
else()
  add_library(lmktrack ${build_src})
  target_link_libraries(lmktrack ${OpenCV_LIBS} ncnn MNN) ##where are ncnn and MNN
endif()

if(BUILD_TRACK_TOOL)
  add_executable(track_tool tests/track_tool.cpp)
  target_link_libraries(track_tool ${OpenCV_LIBS} lmktrack)

  add_executable(infer_test tests/infer_test.cpp )
  target_link_libraries(infer_test ${OpenCV_LIBS} lmktrack)

  add_executable(face_test tests/face_test.cpp src/silent_live.cpp src/silent_live.h)
  target_link_libraries(face_test ${OpenCV_LIBS} lmktrack)
endif()

#if(BUILD_TEST)
#  add_executable(track_test tests/track_test.cpp)
#  target_link_libraries(track_test ${OpenCV_LIBS} lmktrack)
#endif()

if(NOT ANDROID)
 add_executable(c_api_demo samples/c_api_demo.cpp)
 target_link_libraries(c_api_demo ${OpenCV_LIBS} lmktrack MNN)

 add_executable(tracking_demo samples/tracking_demo.cpp)
 target_link_libraries(tracking_demo lmktrack)

 add_executable(tracking_demo_multiprocess samples/tracking_demo_multiprocess.cpp)
 target_link_libraries(tracking_demo_multiprocess lmktrack -lpthread -lm)
endif()


if(BUILD_CHINA_MOBILE)


  # if(BUILD_WITH_ZIP)
  #     add_executable(test_file_reader samples/test_file_reader.cpp src/zip_wrapper.cpp 3rdparty/mini-yaml/yaml/Yaml.cpp )
  #     target_link_libraries(test_file_reader ${OpenCV_LIBS} MNN)
  #   endif()


  #  add_executable(any_tool samples/any_tool.cpp)
  #  target_link_libraries(any_tool ${OpenCV_LIBS} lmktrack MNN)

  #  add_executable(c_api_face_industry samples/c_api_face_industry.cpp)
  #  target_link_libraries(c_api_face_industry ${OpenCV_LIBS} lmktrack MNN)

  #  add_executable(test_face_action_tool samples/test_face_action_tool.cpp)
  #  target_link_libraries(test_face_action_tool ${OpenCV_LIBS} lmktrack MNN)
  
  #  add_executable(test_face_landmark_tool samples/test_face_landmark_tool.cpp)
  #  target_link_libraries(test_face_landmark_tool ${OpenCV_LIBS} lmktrack MNN)
  
  # add_executable(test_face_detect_1_6 samples/test_face_detect_1_6.cpp)
  # target_link_libraries(test_face_detect_1_6 ${OpenCV_LIBS} lmktrack MNN)
  
  #add_executable(test_face_quality_1_11 samples/test_face_quality_1_11.cpp)
  #target_link_libraries(test_face_quality_1_11 ${OpenCV_LIBS} lmktrack MNN)

  #add_executable(test_face_keypoints_1_28 samples/test_face_keypoints_1_28.cpp)
  #target_link_libraries(test_face_keypoints_1_28 ${OpenCV_LIBS} lmktrack MNN)
  
  #add_executable(test_feature_extractor_2_1 samples/test_feature_extractor_2_1.cpp)
  #target_link_libraries(test_feature_extractor_2_1 ${OpenCV_LIBS} lmktrack MNN)
  
  add_executable(test_feature_extractor_1_5w samples/test_feature_extractor_1_5w.cpp)
  target_link_libraries(test_feature_extractor_1_5w ${OpenCV_LIBS} lmktrack MNN)

  #add_executable(test_feature_comparing_sim samples/test_feature_comparing_sim.cpp)
  #target_link_libraries(test_feature_comparing_sim ${OpenCV_LIBS} lmktrack MNN)
  
  #  add_executable(test_compre_feature samples/test_compre_feature.cpp)
  #  target_link_libraries(test_compre_feature ${OpenCV_LIBS} lmktrack MNN)
  
  # add_executable(test_face_liveness_tool samples/test_face_liveness_tool.cpp)
  # target_link_libraries(test_face_liveness_tool ${OpenCV_LIBS} lmktrack MNN)
  
  # add_executable(test_face_liveness_ir_tool samples/test_face_liveness_ir_tool.cpp)
  # target_link_libraries(test_face_liveness_ir_tool ${OpenCV_LIBS} lmktrack MNN)
  
  #  add_executable(test_face_attribute_tool samples/test_face_attribute_tool.cpp)
  #  target_link_libraries(test_face_attribute_tool ${OpenCV_LIBS} lmktrack MNN)
  
  #  add_executable(test_face_quality_tool samples/test_face_quality_tool.cpp)
  #  target_link_libraries(test_face_quality_tool ${OpenCV_LIBS} lmktrack MNN)
  #  
  #  add_executable(test_face_detect_tool samples/test_face_detect_tool.cpp)
  #  target_link_libraries(test_face_detect_tool ${OpenCV_LIBS} lmktrack MNN)
  #  
  #  add_executable(test_feature_extractor_time samples/test_feature_extractor_time.cpp)
  #  target_link_libraries(test_feature_extractor_time ${OpenCV_LIBS} lmktrack MNN)

  # add_executable(test_face_retinaface_tool samples/test_face_retinaface_tool.cpp)
  # target_link_libraries(test_face_retinaface_tool ${OpenCV_LIBS} lmktrack MNN)
  
endif()


