cmake_minimum_required(VERSION 3.10)

project(asic-engine)

add_definitions("-std=c++23 -O0 -ggdb3")
include_directories(include)
include_directories(../mod-loader/include)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(asic-engine SHARED ${SRC_FILES})
set_property(TARGET asic-engine PROPERTY POSITION_INDEPENDENT_CODE ON)
# add_library(cpu-engine_gcov STATIC ${SRC_FILES})
# target_compile_options(cpu-engine_gcov PUBLIC "--coverage")
# target_compile_options(tri-register_gcov PUBLIC "--concepts-diagnostics-depth=10")
