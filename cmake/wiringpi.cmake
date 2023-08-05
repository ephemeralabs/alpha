# TODO(eduponz): alpha copyright notice

# This script adds a CMake function to build the WiringPi library using CMake

function(build_wiringpi)
    # Find threads package: https://cmake.org/cmake/help/v3.2/module/FindThreads.html
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)

    # Get WiringPi sources from the submodule (library only)
    file(GLOB WIRINPI_SRC_FILES ${CMAKE_SOURCE_DIR}/WiringPi/wiringPi/*.c)

    # Build library
    add_library(wiringPi STATIC ${WIRINPI_SRC_FILES})
    target_include_directories(wiringPi PUBLIC ${CMAKE_SOURCE_DIR}/WiringPi/wiringPi/)

    # Link library
    target_link_libraries(wiringPi ${CMAKE_THREAD_LIBS_INIT} crypt m rt)
endfunction()
