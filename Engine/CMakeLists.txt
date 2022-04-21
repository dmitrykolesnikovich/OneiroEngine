cmake_minimum_required(VERSION 3.21)
project(Oneiro)

set(CMAKE_CXX_STANDARD 20)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)

#find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

file(GLOB ONEIRO_SOURCE
        "Source/Core/*.cpp"
        "Source/Renderer/*.cpp" "Source/Renderer/Vulkan/*.cpp"
        "Source/Runtime/*.cpp"
        )
file(GLOB IMGUI_SOURCE
        "../ThirdParty/imgui/*.cpp"
        "../ThirdParty/imgui/backends/imgui_impl_opengl3.cpp"
        "../ThirdParty/imgui/backends/imgui_impl_glfw.cpp"
        )
#file(GLOB OPENGL_SOURCE "../ThirdParty/OpenGL/src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${ONEIRO_SOURCE} )#${IMGUI_SOURCE} ${OPENGL_SOURCE})
add_subdirectory(../ThirdParty/glfw ThirdParty/glfw)
add_subdirectory(../ThirdParty/glm ThirdParty/glm)
add_subdirectory(../ThirdParty/spdlog ThirdParty/spdlog)
#add_subdirectory(../ThirdParty/vulkan-hpp ThirdParty/vulkan-hpp)
#add_subdirectory(../ThirdParty/openal ThirdParty/openal)
target_include_directories(${PROJECT_NAME} PUBLIC ../ThirdParty/OpenGL/include
                                                  ../ThirdParty/stb/include
                                                  ../ThirdParty/imgui
                                                  ../ThirdParty/miniaudio/
                                                  Include/
                                                  )
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ThirdParty/imgui Vulkan::Headers)
target_link_libraries(${PROJECT_NAME} glfw glm spdlog Vulkan::Vulkan) #OpenGL::GL #OpenAL)
target_link_libraries(${CMAKE_PROJECT_NAME} Oneiro)
