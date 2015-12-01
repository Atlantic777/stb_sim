cmake_minimum_required(VERSION 2.8)
project(STB_sim C)

set(OUT_PREFIX "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_PREFIX}")

if(CROSS_COMPILE)
  message("Doing cross compile")
  include(cross_toolchain.cmake)
endif()

include_directories("inc")
include_directories("cutest/include")

add_subdirectory("src")
add_subdirectory("tests")
add_subdirectory("doc")

