cmake_minimum_required(VERSION 3.10)

project(modules)

add_subdirectory(common)
include_directories(common/include)

set(mods_dir "${PROJECT_SOURCE_DIR}/modules")

FILE(GLOB subdirs RELATIVE ${mods_dir} "${mods_dir}/*")

foreach(subdir ${subdirs})
    if (IS_DIRECTORY "${mods_dir}/${subdir}")
        add_subdirectory("${mods_dir}/${subdir}")
    endif()
endforeach()
