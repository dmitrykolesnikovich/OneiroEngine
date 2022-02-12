cmake_minimum_required(VERSION 3.21)
project(Engine.Renderer)

set(CMAKE_CXX_STANDARD 14)

file(GLOB ${PROJECT_NAME}_SRC "Source/OpenGL/*.cpp")

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
add_subdirectory(../ThirdParty/OpenGL/ OpenGL)
target_link_libraries(${PROJECT_NAME} OpenGL::Loader)
target_include_directories(${PROJECT_NAME} PUBLIC Include/ ../ThirdParty/stb/include ../ThirdParty/DZL/include ../Debugger/Include ../ThirdParty/glm)