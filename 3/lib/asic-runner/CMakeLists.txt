cmake_minimum_required(VERSION 3.10)

project(asic-runner)

add_definitions("-std=c++23 -O0 -ggdb3")
include_directories(include)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(asic-runner SHARED ${SRC_FILES})
set_property(TARGET asic-runner PROPERTY POSITION_INDEPENDENT_CODE ON)
