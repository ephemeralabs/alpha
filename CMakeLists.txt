# TODO(eduponz): alpha copyright notice

# Setup CMake project
cmake_minimum_required(VERSION 3.0)
project(alpha)

# Build WiringPi
add_subdirectory(${PROJECT_SOURCE_DIR}/wiringpi)
set(WIRINGPI_FOUND TRUE)

# Build app
add_executable(alpha src/main.cpp)
target_link_libraries(alpha wiringpi)
