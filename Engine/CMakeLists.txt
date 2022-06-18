cmake_minimum_required(VERSION 3.21)
project(Oneiro)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_IMGUI ON CACHE BOOL "" FORCE)
set(BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTENSIONS OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)

if (OE_API_VULKAN)
    message(FATAL_ERROR "Currently Oneiro Engine dont support Vulkan API")
else ()
    find_package(OpenGL REQUIRED)
    file(GLOB OPENGL_SOURCE "../ThirdParty/OpenGL/src/*.cpp")
    file(GLOB IMGUI_SOURCE
            "../ThirdParty/imgui/*.cpp"
            "../ThirdParty/imgui/backends/imgui_impl_opengl3.cpp"
            "../ThirdParty/imgui/backends/imgui_impl_glfw.cpp"
            )
    file(GLOB ONEIRO_RENDERER_SOURCE
            "Source/Renderer/OpenGL/*.cpp"
            )
endif ()

file(GLOB ONEIRO_SOURCE
        "Source/Core/*.cpp"
        "Source/Renderer/*.cpp"
        "Source/Renderer/Gui/*.cpp"
        "Source/Runtime/*.cpp"
        "Source/World/*.cpp"
        "Source/VisualNovel/*.cpp"
        )

add_library(${PROJECT_NAME} STATIC ${ONEIRO_SOURCE} ${IMGUI_SOURCE} ${OPENGL_SOURCE} ${ONEIRO_RENDERER_SOURCE})
add_subdirectory(../ThirdParty/glfw ThirdParty/glfw)
add_subdirectory(../ThirdParty/glm ThirdParty/glm)
add_subdirectory(../ThirdParty/spdlog ThirdParty/spdlog)
add_subdirectory(../ThirdParty/entt ThirdParty/entt)
add_subdirectory(../ThirdParty/yaml-cpp ThirdParty/yaml-cpp)
add_subdirectory(../ThirdParty/sol2 ThirdParty/sol2)
add_subdirectory(../ThirdParty/lua ThirdParty/lua)
add_subdirectory(../ThirdParty/HazelAudio ThirdParty/HazelAudio)
add_subdirectory(../ThirdParty/freetype ThirdParty/freetype)
add_subdirectory(../ThirdParty/assimp ThirdParty/assimp)

target_include_directories(${PROJECT_NAME} PUBLIC ../ThirdParty/stb/include ../ThirdParty/imgui/ Include/ ../ThirdParty/OpenGL/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../ThirdParty/imgui)
target_link_libraries(${PROJECT_NAME} glfw glm spdlog EnTT yaml-cpp::yaml-cpp lua_static
        sol2::sol2 Hazel.Audio freetype assimp::assimp)
if (OE_API_VULKAN)
    # Link Vulkan...
else ()
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif ()
target_link_libraries(${CMAKE_PROJECT_NAME} Oneiro)
