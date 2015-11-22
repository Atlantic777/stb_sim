cmake_minimum_required(VERSION 2.8)
project(STB_sim C)

set(OUT_PREFIX "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_PREFIX}")

include_directories("inc")
add_subdirectory("src")
