cmake_minimum_required(VERSION 3.15.3)
project(siol VERSION 0.1.0)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g3 -ggdb")

aux_source_directory(${PROJECT_SOURCE_DIR} src)
add_executable(siol ${src})