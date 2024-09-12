cmake_minimum_required(VERSION 3.10)

project(lab1 VERSION 0.0.1 LANGUAGES CXX)

add_definitions("-ggdb3 -O3 -std=c++20")

include_directories(include)
include_directories(lib/syllables_divider)
include_directories(lib/ioutil)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_subdirectory(lib/syllables_divider)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

option(DOXYGEN "create documentation" OFF)
option(TESTING "compile binaries for testing" OFF)

add_executable(lab1 ${SRC_FILES} main.cpp)
target_link_libraries(lab1 syllables_divider)

if (DOXYGEN)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM)
    else (DOXYGEN_FOUND)
      message(
          "Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif (DOXYGEN)

# tests

if (TESTING)
    find_package(Catch2 REQUIRED)
    file(GLOB_RECURSE TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

    set (ALL_TESTS all_tests)

    add_executable(${ALL_TESTS} ${TEST_FILES} ${SRC_FILES})
    target_compile_options(${ALL_TESTS} PUBLIC "--coverage")
    target_link_libraries(${ALL_TESTS} PRIVATE
                          gcov
                          syllables_divider_gcov
                          Catch2::Catch2WithMain)
    add_test(NAME ${ALL_TESTS} COMMAND ${ALL_TESTS})
endif (TESTING)

# Get compile_commands.jons

set(cmp_cmd_file "compile_commands.json")
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${cmp_cmd_file})
    configure_file(${CMAKE_CURRENT_BINARY_DIR}/${cmp_cmd_file}
                   ${CMAKE_CURRENT_SOURCE_DIR}/${cmp_cmd_file}
                   COPYONLY)
endif()
