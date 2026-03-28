cmake_minimum_required(VERSION 3.15)

project(rdk)

set(CMAKE_CXX_STANDARD 17)

# RDK Library

add_library(rdk INTERFACE)

target_include_directories(rdk INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
