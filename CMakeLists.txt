cmake_minimum_required(VERSION 2.8)
project(ORB_SLAM2)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(OpenCV REQUIRED)
#find_package(Eigen3  REQUIRED)
find_package(Pangolin REQUIRED)

include_directories(
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/include/include
${EIGEN3_INCLUDE_DIR}
${Pangolin_INCLUDE_DIRS}
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
add_executable(RGBD
./RGBD.cpp inuitive.cpp)
target_link_libraries(RGBD
${OpenCV_LIBS}
${EIGEN3_LIBS}
${Pangolin_LIBRARIES}
rt
${PROJECT_SOURCE_DIR}/lib/libDBoW2.so
${PROJECT_SOURCE_DIR}/lib/libg2o.so
${PROJECT_SOURCE_DIR}/lib/libORB_SLAM2.so
${PROJECT_SOURCE_DIR}/lib/libInuStreams.so 
${PROJECT_SOURCE_DIR}/lib/libCommonUtilities.so
)

file(COPY ORBvoc.bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY TUM1.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
