cmake_minimum_required(VERSION 3.21)
project(Oneiro)

set(CMAKE_CXX_STANDARD 20)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)

find_package(Vulkan REQUIRED)

file(GLOB SHADERFILES "Shaders/*.spv")
file(COPY ${SHADERFILES} DESTINATION ${BINARY_DIRECTORY}/Shaders/)

file(GLOB ONEIRO_SOURCE
        "Source/Core/*.cpp"
        "Source/Renderer/*.cpp" "Source/Renderer/Vulkan/*.cpp"
        "Source/Runtime/*.cpp"
        )
file(GLOB IMGUI_SOURCE
        "../ThirdParty/imgui/*.cpp"
        "../ThirdParty/imgui/backends/imgui_impl_glfw.cpp"
        )

add_library(${PROJECT_NAME} STATIC ${ONEIRO_SOURCE})
add_subdirectory(../ThirdParty/glfw ThirdParty/glfw)
add_subdirectory(../ThirdParty/glm ThirdParty/glm)
add_subdirectory(../ThirdParty/spdlog ThirdParty/spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ../ThirdParty/stb/include
                                                  Include/
                                                  )
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ThirdParty/imgui Vulkan::Headers)
target_link_libraries(${PROJECT_NAME} glfw glm spdlog Vulkan::Vulkan)
target_link_libraries(${CMAKE_PROJECT_NAME} Oneiro)
