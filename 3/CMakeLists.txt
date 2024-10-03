cmake_minimum_required(VERSION 3.10)

project(lab1 VERSION 0.0.1 LANGUAGES CXX)

add_definitions("-ggdb3 -O0 -std=c++23")

include_directories(lib/include)
include_directories(lib/thread-safe-containers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_subdirectory(lib)
# add_subdirectory(lib/tri-register)


option(DOXYGEN "create documentation" OFF)
option(TESTING "compile binaries for testing" OFF)
option(RVPROC "compile test ricsc-v proccessor")

if (RVPROC)
    find_package(nlohmann_json 3.2.0 REQUIRED)
    file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/prog/*.cpp")
    add_executable(proc ${SRC_FILES})
    target_include_directories(proc PUBLIC prog/include)
    target_include_directories(proc PUBLIC lib/asic-engine/include)
    target_include_directories(proc PUBLIC lib/logger/include)
    target_include_directories(proc PUBLIC lib/mod-loader/include)
    # TODO foreach
    target_include_directories(proc PUBLIC lib/modules/common/include)
    target_include_directories(proc PUBLIC lib/modules/modules/default-control-device/include)
    target_include_directories(proc PUBLIC lib/modules/modules/switch-anypkt/include)
    target_include_directories(proc PUBLIC lib/modules/modules/default-device-anypkt-ipkt/include)
    target_include_directories(proc PUBLIC lib/modules/modules/port-rw1248i-ipkt/include)
    target_link_libraries(proc PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(proc PUBLIC asic-engine)
    target_link_libraries(proc PUBLIC mod-loader)
    target_link_libraries(proc PUBLIC roma004-logger)
    target_link_libraries(proc PUBLIC modules-common)
endif (RVPROC)

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

# if (TESTING)
#     find_package(Catch2 REQUIRED)
#     # file(GLOB_RECURSE TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
# 
#     set(TEST_FILES
#         # tests/test_master_bus.cpp
#         tests/test_cpu_base_instruction.cpp
#         tests/test_cpu_generic_instruction.cpp
#         tests/test_cpu_registers_set.cpp
#         tests/test_cpu_register_request.cpp
#         tests/test_cpu_register_request_manager.cpp
#         tests/test_cpu_default_executor_device.cpp
#         tests/test_cpu_data_master_port.cpp
#         tests/test_dm_switch_context.cpp
#         tests/test_dm_interconnect_packet.cpp
#         tests/test_dm_packet_queue.cpp
#         tests/test_dm_packet_receiver.cpp
#         tests/test_dm_packet_sender.cpp
#         tests/test_dm_device_gate.cpp
#         tests/test_dm_cpu_gate.cpp
#         tests/test_dm_default_device.cpp
#         tests/test_dm_switch.cpp
#     )
# 
#     set (ALL_TESTS all_tests)
# 
#     add_executable(${ALL_TESTS} ${TEST_FILES} ${SRC_FILES})
#     target_compile_options(${ALL_TESTS} PUBLIC "--coverage")
#     target_link_libraries(${ALL_TESTS} PRIVATE
#                           gcov
#                           cpu-engine_gcov
#                           Catch2::Catch2WithMain)
#     add_test(NAME ${ALL_TESTS} COMMAND ${ALL_TESTS})
# endif (TESTING)

# Get compile_commands.jons

set(cmp_cmd_file "compile_commands.json")
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${cmp_cmd_file})
    configure_file(${CMAKE_CURRENT_BINARY_DIR}/${cmp_cmd_file}
                   ${CMAKE_CURRENT_SOURCE_DIR}/${cmp_cmd_file}
                   COPYONLY)
endif()
