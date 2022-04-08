cmake_minimum_required(VERSION 3.21)
project(Oneiro)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

file(GLOB ONEIRO_SOURCE
        "Source/Core/*.cpp"
        "Source/Renderer/*.cpp" "Source/Renderer/OpenGL/*.cpp"
        "Source/Runtime/*.cpp"
        "ThirdParty/OpenGL/src/*.cpp"
        )
file(GLOB IMGUI_SOURCE
        "ThirdParty/imgui/*.cpp"
        "ThirdParty/imgui/backends/imgui_impl_opengl3.cpp"
        "ThirdParty/imgui/backends/imgui_impl_glfw.cpp"
        )

add_library(${PROJECT_NAME} STATIC ${ONEIRO_SOURCE} ${IMGUI_SOURCE})
add_subdirectory(ThirdParty/glfw ThirdParty/glfw)
add_subdirectory(ThirdParty/glm ThirdParty/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ThirdParty/OpenGL/include
                                                  ThirdParty/stb/include
                                                  Include/
                                                  ThirdParty/imgui
                                                  )
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ThirdParty/imgui)
target_link_libraries(${PROJECT_NAME} glfw glm OpenGL::GL)
target_link_libraries(${CMAKE_PROJECT_NAME} Oneiro)
