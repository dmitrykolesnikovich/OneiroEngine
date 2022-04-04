cmake_minimum_required(VERSION 3.21)
project(Oneiro.Core)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB ${PROJECT_NAME}_SRC "Source/*.cpp")

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
add_library(Oneiro::Core ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(${PROJECT_NAME} PUBLIC Include/)

add_subdirectory(../ThirdParty/glfw GLFW)
target_link_libraries(${PROJECT_NAME} glfw)