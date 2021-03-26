cmake_minimum_required(VERSION 3.1)

project(cloud-load-balancing-simulator)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -Wall -Os")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os")

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
