cmake_minimum_required(VERSION 3.5)
project(canager)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main.c
    LICENSE
    README.md src/data_struct.h src/functions.h)

include_directories(.)
include_directories(bin)

add_executable(canager ${SOURCE_FILES} src/main.c)