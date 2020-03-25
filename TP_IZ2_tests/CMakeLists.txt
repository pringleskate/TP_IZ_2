
cmake_minimum_required(VERSION 3.15)
project(TP_IZ2_tests)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror -Wpedantic")

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

include_directories(../TP_IZ_2_single_thread/inc ../TP_IZ_2_multi_thread/inc)
link_directories(../TP_IZ_2_single_thread/cmake-build-debug ../TP_IZ_2_multi_thread/cmake-build-debug)

add_executable(TP_IZ2_tests main.cpp tests.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(TP_IZ2_tests gtest_main libTP_IZ_2_single_thread.dylib libTP_IZ_2_multi_thread.a)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(TP_IZ2_tests gtest_main libTP_IZ_2_single_thread.so libTP_IZ_2_multi_thread.a)
endif()