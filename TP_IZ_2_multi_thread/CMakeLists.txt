cmake_minimum_required(VERSION 3.15)
project(TP_IZ_2_multi_thread C)

set(CMAKE_C_STANDARD 11)

add_library(TP_IZ_2_multi_thread inc/library_static.h src/library_static.c src/process_dirs.c src/metrics.c src/helpful_functions.c)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_compile_definitions(TP_IZ_2_multi_thread PUBLIC "IS_MACOS")
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_compile_definitions(TP_IZ_2_multi_thread PUBLIC "IS_LINUX")
endif()

#add_library(multi_search src/multi_search.c include/multi_search.h)
