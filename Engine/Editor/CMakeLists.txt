cmake_minimum_required(VERSION 3.21)
project(Engine.Editor)

set(CMAKE_CXX_STANDARD 14)

file(GLOB ${PROJECT_NAME}_SRC
        "Source/*.cpp"
        )

include_directories(../ThirdParty/DZL/include)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC
        Include/
        )