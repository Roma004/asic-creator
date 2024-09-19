cmake_minimum_required(VERSION 3.10)

project(syllables_divider)

add_definitions("-ggdb3 -O0 -std=c++20")

add_library(syllables_divider STATIC syllables_divider.cpp)

add_library(syllables_divider_gcov STATIC syllables_divider.cpp)
target_compile_options(syllables_divider_gcov PUBLIC "--coverage")
