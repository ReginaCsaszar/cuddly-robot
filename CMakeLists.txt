cmake_minimum_required(VERSION 3.8)
project(cuddly_robot)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(cuddly_robot ${SOURCE_FILES})