cmake_minimum_required(VERSION 3.20)
project(DZL_VECTOR_TEST)

set(CMAKE_CXX_STANDARD 14)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCE_DIR}/main.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/../../ DZL)

target_link_libraries(${CMAKE_PROJECT_NAME}
        DZL)