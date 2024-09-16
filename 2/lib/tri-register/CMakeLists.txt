cmake_minimum_required(VERSION 3.10)

project(tri-register)

add_definitions("-std=c++23 -O0 -ggdb3")

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# set (SRC_FILES
#     src/trc-const-reference.cpp
#     src/trc-reference.cpp
#     src/trc-reg-chunk.cpp
#     src/trc.cpp
# )

add_library(tri-register STATIC ${SRC_FILES})

add_library(tri-register_gcov STATIC ${SRC_FILES})
target_compile_options(tri-register_gcov PUBLIC "--coverage")
# target_compile_options(tri-register_gcov PUBLIC "--concepts-diagnostics-depth=10")
