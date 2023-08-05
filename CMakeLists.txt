# TODO(eduponz): alpha copyright notice

# Setup CMake project
cmake_minimum_required(VERSION 3.0)
project(alpha)

# Build WiringPi
include(${CMAKE_SOURCE_DIR}/cmake/wiringpi.cmake)
build_wiringpi()
