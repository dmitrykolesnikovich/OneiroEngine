cmake_minimum_required(VERSION 3.20)
PROJECT(DZL)

set(CMAKE_CXX_STANDARD 14)

file(GLOB DZLSRC
        "include/DZL/*.hpp"
        )
add_library(DZL INTERFACE ${DZLSRC})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(DZL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)