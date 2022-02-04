#!/bin/bash

set -ex

RELEASE_HOME=$(cd $(dirname $0)/../..; pwd)
BUILD_DIR=${RELEASE_HOME}/release_android

[[ -d ${BUILD_DIR} ]] && rm -r ${BUILD_DIR}

build() {
    arch=$1
    NDK_API_LEVEL=$2
    mkdir -p ${BUILD_DIR}/${arch}
    pushd ${BUILD_DIR}/${arch}
    cmake ${RELEASE_HOME} \
        -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID_TOOLCHAIN=clang \
        -DANDROID_ABI=${arch} \
        -DANDROID_NATIVE_API_LEVEL=${NDK_API_LEVEL} \
        -DANDROID_STL=c++_static \
        -DBUILD_CUDA=OFF \
        -DOpenCV_DIR=${OPENCV_DIR} \
        -DNCNN_DIR=${RELEASE_HOME}/3rdparty/ncnn/android/${arch} \
        -DMNN_DIR=${RELEASE_HOME}/3rdparty/MNN-release/android/${arch} 
    make -j$(nproc) lmktrack
    ls ${BUILD_DIR}/${arch}| grep -v so| xargs rm -r
    #make -j$(nproc) track_tool
    popd
}


build arm64-v8a 21
build armeabi-v7a 21

cp ${RELEASE_HOME}/src/c_api/c_api.h ${RELEASE_HOME}/release_android/c_api.h
mkdir ${RELEASE_HOME}/release_android/model
cp  ${RELEASE_HOME}/tools/pack/face_im_e* ${RELEASE_HOME}/release_android/model
cp  ${RELEASE_HOME}/tools/pack/face_tracker.bin ${RELEASE_HOME}/release_android/model
date -R > ${BUILD_DIR}/release_note.txt
cd ${BUILD_DIR}
find . -type f |xargs md5sum >>release_note.txt
cd -
#cp -r ${RELEASE_HOME}/samples/c_api_demo.cpp ${RELEASE_HOME}/release_android
#cp -r ${RELEASE_HOME}/samples/utils ${RELEASE_HOME}/release_android
