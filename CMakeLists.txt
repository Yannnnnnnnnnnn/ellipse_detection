cmake_minimum_required(VERSION 2.8)

PROJECT(Circle)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


find_package(OpenCV REQUIRED)	
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(. DIR_SRCS)

add_executable(Circle ${DIR_SRCS})
target_link_libraries(Circle ${OpenCV_LIBS})




