cmake_minimum_required(VERSION 3.10)

project(logger)

add_definitions("-std=c++23 -O0 -ggdb3")
include_directories(include)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(roma004-logger SHARED ${SRC_FILES})
