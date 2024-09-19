cmake_minimum_required(VERSION 3.10)

project(tristate)

add_definitions("-std=c++23 -ggdb3 -O0")

add_library(tristate STATIC tristate.cpp)

add_library(tristate_gcov STATIC tristate.cpp)
target_compile_options(tristate_gcov PUBLIC "--coverage")
