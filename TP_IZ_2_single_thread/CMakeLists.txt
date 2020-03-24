cmake_minimum_required(VERSION 3.15)
project(TP_IZ_2_single_thread C)

set(CMAKE_C_STANDARD 11)

add_library(TP_IZ_2_single_thread SHARED src/library.c inc/library.h src/process_dirs.c src/metrics.c src/helpful_functions.c)