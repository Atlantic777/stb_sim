cmake_minimum_required(VERSION 2.8)
project(STB_sim C)

include(cross_toolchain.cmake)

set(OUT_PREFIX "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_PREFIX}")

include_directories("inc")
include_directories("cutest/include")

add_subdirectory("src")
add_subdirectory("tests")
