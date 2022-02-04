# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /app_sdk/cmake-3.15.4-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /app_sdk/cmake-3.15.4-Linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /app_sdk/nazhi_v5/nz_face_dev

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /app_sdk/nazhi_v5/nz_face_dev/build

# Include any dependencies generated for this target.
include CMakeFiles/lmktrack.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lmktrack.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lmktrack.dir/flags.make

CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.o: ../src/detector/retinafacev2_mnn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/detector/retinafacev2_mnn.cpp

CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/detector/retinafacev2_mnn.cpp > CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.i

CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/detector/retinafacev2_mnn.cpp -o CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.s

CMakeFiles/lmktrack.dir/src/mtcnn.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/mtcnn.cpp.o: ../src/mtcnn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lmktrack.dir/src/mtcnn.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/mtcnn.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/mtcnn.cpp

CMakeFiles/lmktrack.dir/src/mtcnn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/mtcnn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/mtcnn.cpp > CMakeFiles/lmktrack.dir/src/mtcnn.cpp.i

CMakeFiles/lmktrack.dir/src/mtcnn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/mtcnn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/mtcnn.cpp -o CMakeFiles/lmktrack.dir/src/mtcnn.cpp.s

CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.o: ../src/mtcnn_mnn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/mtcnn_mnn.cpp

CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/mtcnn_mnn.cpp > CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.i

CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/mtcnn_mnn.cpp -o CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.s

CMakeFiles/lmktrack.dir/src/model_loader.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/model_loader.cpp.o: ../src/model_loader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/lmktrack.dir/src/model_loader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/model_loader.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/model_loader.cpp

CMakeFiles/lmktrack.dir/src/model_loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/model_loader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/model_loader.cpp > CMakeFiles/lmktrack.dir/src/model_loader.cpp.i

CMakeFiles/lmktrack.dir/src/model_loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/model_loader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/model_loader.cpp -o CMakeFiles/lmktrack.dir/src/model_loader.cpp.s

CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.o: ../src/landmark_tracker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/landmark_tracker.cpp

CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/landmark_tracker.cpp > CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.i

CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/landmark_tracker.cpp -o CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.s

CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.o: ../src/c_api/c_api.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.o -c /app_sdk/nazhi_v5/nz_face_dev/src/c_api/c_api.cc

CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/c_api/c_api.cc > CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.i

CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/c_api/c_api.cc -o CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.s

CMakeFiles/lmktrack.dir/src/feature_extract.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/feature_extract.cpp.o: ../src/feature_extract.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/lmktrack.dir/src/feature_extract.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/feature_extract.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/feature_extract.cpp

CMakeFiles/lmktrack.dir/src/feature_extract.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/feature_extract.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/feature_extract.cpp > CMakeFiles/lmktrack.dir/src/feature_extract.cpp.i

CMakeFiles/lmktrack.dir/src/feature_extract.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/feature_extract.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/feature_extract.cpp -o CMakeFiles/lmktrack.dir/src/feature_extract.cpp.s

CMakeFiles/lmktrack.dir/src/silent_live.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/silent_live.cpp.o: ../src/silent_live.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/lmktrack.dir/src/silent_live.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/silent_live.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/silent_live.cpp

CMakeFiles/lmktrack.dir/src/silent_live.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/silent_live.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/silent_live.cpp > CMakeFiles/lmktrack.dir/src/silent_live.cpp.i

CMakeFiles/lmktrack.dir/src/silent_live.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/silent_live.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/silent_live.cpp -o CMakeFiles/lmktrack.dir/src/silent_live.cpp.s

CMakeFiles/lmktrack.dir/src/face_attribute.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/face_attribute.cpp.o: ../src/face_attribute.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/lmktrack.dir/src/face_attribute.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/face_attribute.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/face_attribute.cpp

CMakeFiles/lmktrack.dir/src/face_attribute.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/face_attribute.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/face_attribute.cpp > CMakeFiles/lmktrack.dir/src/face_attribute.cpp.i

CMakeFiles/lmktrack.dir/src/face_attribute.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/face_attribute.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/face_attribute.cpp -o CMakeFiles/lmktrack.dir/src/face_attribute.cpp.s

CMakeFiles/lmktrack.dir/src/face_quality.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/face_quality.cpp.o: ../src/face_quality.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/lmktrack.dir/src/face_quality.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/face_quality.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/face_quality.cpp

CMakeFiles/lmktrack.dir/src/face_quality.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/face_quality.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/face_quality.cpp > CMakeFiles/lmktrack.dir/src/face_quality.cpp.i

CMakeFiles/lmktrack.dir/src/face_quality.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/face_quality.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/face_quality.cpp -o CMakeFiles/lmktrack.dir/src/face_quality.cpp.s

CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.o: ../src/silent_live_ir.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/silent_live_ir.cpp

CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/silent_live_ir.cpp > CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.i

CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/silent_live_ir.cpp -o CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.s

CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.o: ../3rdparty/mini-yaml/yaml/Yaml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/3rdparty/mini-yaml/yaml/Yaml.cpp

CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/3rdparty/mini-yaml/yaml/Yaml.cpp > CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.i

CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/3rdparty/mini-yaml/yaml/Yaml.cpp -o CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.s

CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.o: ../src/zip_wrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/zip_wrapper.cpp

CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/zip_wrapper.cpp > CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.i

CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/zip_wrapper.cpp -o CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.s

CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.o: ../src/detector/face_detector_retina.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/detector/face_detector_retina.cpp

CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/detector/face_detector_retina.cpp > CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.i

CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/detector/face_detector_retina.cpp -o CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.s

CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.o: CMakeFiles/lmktrack.dir/flags.make
CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.o: ../src/detector/face_detector_mnn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.o -c /app_sdk/nazhi_v5/nz_face_dev/src/detector/face_detector_mnn.cpp

CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /app_sdk/nazhi_v5/nz_face_dev/src/detector/face_detector_mnn.cpp > CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.i

CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /app_sdk/nazhi_v5/nz_face_dev/src/detector/face_detector_mnn.cpp -o CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.s

# Object files for target lmktrack
lmktrack_OBJECTS = \
"CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.o" \
"CMakeFiles/lmktrack.dir/src/mtcnn.cpp.o" \
"CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.o" \
"CMakeFiles/lmktrack.dir/src/model_loader.cpp.o" \
"CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.o" \
"CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.o" \
"CMakeFiles/lmktrack.dir/src/feature_extract.cpp.o" \
"CMakeFiles/lmktrack.dir/src/silent_live.cpp.o" \
"CMakeFiles/lmktrack.dir/src/face_attribute.cpp.o" \
"CMakeFiles/lmktrack.dir/src/face_quality.cpp.o" \
"CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.o" \
"CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.o" \
"CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.o" \
"CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.o" \
"CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.o"

# External object files for target lmktrack
lmktrack_EXTERNAL_OBJECTS =

liblmktrack.so: CMakeFiles/lmktrack.dir/src/detector/retinafacev2_mnn.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/mtcnn.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/mtcnn_mnn.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/model_loader.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/landmark_tracker.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/c_api/c_api.cc.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/feature_extract.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/silent_live.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/face_attribute.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/face_quality.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/silent_live_ir.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/3rdparty/mini-yaml/yaml/Yaml.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/zip_wrapper.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/detector/face_detector_retina.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/src/detector/face_detector_mnn.cpp.o
liblmktrack.so: CMakeFiles/lmktrack.dir/build.make
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.9
liblmktrack.so: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
liblmktrack.so: CMakeFiles/lmktrack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX shared library liblmktrack.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lmktrack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lmktrack.dir/build: liblmktrack.so

.PHONY : CMakeFiles/lmktrack.dir/build

CMakeFiles/lmktrack.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lmktrack.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lmktrack.dir/clean

CMakeFiles/lmktrack.dir/depend:
	cd /app_sdk/nazhi_v5/nz_face_dev/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /app_sdk/nazhi_v5/nz_face_dev /app_sdk/nazhi_v5/nz_face_dev /app_sdk/nazhi_v5/nz_face_dev/build /app_sdk/nazhi_v5/nz_face_dev/build /app_sdk/nazhi_v5/nz_face_dev/build/CMakeFiles/lmktrack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lmktrack.dir/depend

