cmake_minimum_required(VERSION 3.21)
project(Engine.Renderer)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

file(GLOB ${PROJECT_NAME}_SRC "Source/OpenGL/*.cpp" "Source/*.cpp" "../ThirdParty/OpenGL/src/gl_core_4_5.cpp")

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_include_directories(${PROJECT_NAME} PUBLIC
        Include/ ../ThirdParty/stb/include
        ../ThirdParty/DZL/include ../Debugger/Include
        ../ThirdParty/glm ../ThirdParty/OpenGL/include/
        )