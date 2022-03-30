cmake_minimum_required(VERSION 3.21)
project(Oneiro.VisualNovel)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ${PROJECT_NAME}_SRC "Source/*.cpp")

include_directories(../ThirdParty/DZL/include)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC} "../ThirdParty/OpenGL/src/gl_core_4_5.cpp")
add_library(Oneiro::VisualNovel ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(${PROJECT_NAME} PUBLIC Include/)

add_subdirectory(../Core/ Oneiro/Core)
add_subdirectory(../Renderer/ Oneiro/Renderer)
target_link_libraries(${PROJECT_NAME} Oneiro::Core Oneiro::Renderer)

