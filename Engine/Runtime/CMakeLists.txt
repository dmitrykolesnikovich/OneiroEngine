cmake_minimum_required(VERSION 3.21)
project(Engine.Runtime)

set(CMAKE_CXX_STANDARD 14)
set(ENGINE ON)

file(GLOB ${PROJECT_NAME}_SRC "Source/*.cpp")

include_directories(../ThirdParty/DZL/include)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(${PROJECT_NAME} PUBLIC Include/)
add_subdirectory(../Core/ Core)
add_subdirectory(../Debugger/ Debugger)
add_subdirectory(../Renderer/ Renderer)
add_subdirectory(../Editor/ Editor)
target_link_libraries(${PROJECT_NAME}
        Engine.Core
        Engine.Debugger
        Engine.Renderer
        Engine.Editor
        )