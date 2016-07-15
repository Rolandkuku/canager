cmake_minimum_required(VERSION 3.5)
project(canager)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES
    src/main.c
    LICENSE
    README.md src/data_struct.h src/functions.h src/team.h src/project.h)


add_executable(canager ${SOURCE_FILES} src/main.c)

include_directories(.)
include_directories(bin, src)
find_package(libxml2)
if(LIBXML2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIR})
    target_link_libraries(canager ${LIBXML2_LIBRARIES})
endif(LIBXML2_FOUND)
