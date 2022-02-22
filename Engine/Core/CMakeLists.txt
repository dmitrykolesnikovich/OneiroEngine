cmake_minimum_required(VERSION 3.21)
project(Oneiro.Core)

set(CMAKE_CXX_STANDARD 14)

file(GLOB ${PROJECT_NAME}_SRC "Source/*.cpp")

include_directories(../ThirdParty/DZL/include)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
add_library(Oneiro::Core ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(${PROJECT_NAME} PUBLIC Include/)

add_subdirectory(../ThirdParty/glfw GLFW)
target_link_libraries(${PROJECT_NAME} glfw)

