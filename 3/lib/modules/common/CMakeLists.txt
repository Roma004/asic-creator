cmake_minimum_required(VERSION 3.10)

project(modules-common)

add_definitions("-ggdb3 -O0 -std=c++23")

include_directories(include)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(modules-common SHARED ${SRC_FILES})
target_link_libraries(modules-common asic-engine)
