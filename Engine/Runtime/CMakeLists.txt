cmake_minimum_required(VERSION 3.21)
project(Oneiro.Runtime)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ${PROJECT_NAME}_SRC "Source/*.cpp")

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
add_library(Oneiro::Runtime ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(${PROJECT_NAME} PUBLIC Include/)

add_subdirectory(../VisualNovel/ Oneiro/VisualNovel)
target_link_libraries(${PROJECT_NAME} Oneiro::VisualNovel)