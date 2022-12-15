cmake_minimum_required(VERSION 3.24)
project(Jigsaw-Solver)

set(CMAKE_CXX_STANDARD 17)

#find_library(OpenCV)
find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/usr/local/Cellar/opencv/4.6.0_1/include/opencv4/opencv2/")

set(CMAKE_CXX_STANDARD 11)

add_executable(Jigsaw-Solver src/main.cpp src/jigsaw_maker.h src/jigsaw_maker.cpp src/solver.h src/solver.cpp src/input_struct.h src/input_struct.cpp)

target_link_libraries(Jigsaw-Solver ${OpenCV_LIBS})