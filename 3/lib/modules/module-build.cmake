cmake_minimum_required(VERSION 3.10)

set(full_name "${name}-${version}")

project(${name} VERSION ${version})

add_definitions("-std=c++23 -O0 -ggdb3")

include_directories(include)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/*.cpp")

add_library(${full_name} SHARED ${SRC_FILES})

# # uncomment to link with common directory
add_dependencies(${full_name} modules-common)
target_link_libraries(modules-common)

set(LIBCPU asic-engine)
set(LIBMODLOADER mod-loader)
set(LIBLOGGER roma004-logger)

target_link_libraries(${full_name} PRIVATE ${LIBCPU} ${LIBMODLOADER} ${LIBLOGGER})

set_property(TARGET ${full_name} PROPERTY POSITION_INDEPENDENT_CODE ON)
