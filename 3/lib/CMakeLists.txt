cmake_minimum_required(VERSION 3.10)

project(lib)

include_directories(asic-engine/include)
include_directories(mod-loader/include)
include_directories(logger/include)

add_subdirectory(asic-engine)
add_subdirectory(mod-loader)
add_subdirectory(logger)

# TODO requires libs from previouse ones
add_subdirectory(modules)
