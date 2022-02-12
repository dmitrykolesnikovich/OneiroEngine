//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Core/Window.hpp"

bool oe::Core::Window::Create(dzl::uint16_t width, dzl::uint16_t height, const dzl::string& title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

    glfwMakeContextCurrent(mWindow);

    if (mWindow == nullptr)
        return false;

    return true;
}
oe::Core::Window::~Window()
{
    glfwDestroyWindow(mWindow);
}
bool oe::Core::Window::isClosed()
{
    return glfwWindowShouldClose(mWindow);
}
void oe::Core::Window::PollEvents()
{
    glfwPollEvents();
}

void oe::Core::Window::Update()
{
    glfwSwapBuffers(mWindow);
}
