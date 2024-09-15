cmake_minimum_required(VERSION 3.10)

project(tri-register)

add_definitions("-std=c++23 -O3 -ggdb3")

add_library(tri-register STATIC tri-register.cpp)

add_library(tri-register_gcov STATIC tri-register.cpp)
target_compile_options(tri-register_gcov PUBLIC "--coverage")
