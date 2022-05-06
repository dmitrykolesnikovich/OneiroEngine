//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Window.hpp"

#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Core
{
    Window::~Window()
    {
        glfwDestroyWindow(mWindow);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::WaitEvents()
    {
        glfwWaitEvents();
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(mWindow);
    }

    bool Window::IsClosed() const
    {
        return glfwWindowShouldClose(mWindow) != 0;
    }

    GLFWwindow* Window::GetGLFW() const
    {
        return mWindow;
    }

    void Window::SetSize(int width, int height)
    {
        mData.Width = width;
        mData.Height = height;
    }

    const char* Window::GetTitle() const
    {
        return mData.Title;
    }

    float Window::GetAr() const
    {
        return mData.AR;
    }

    int Window::GetWidth() const
    {
        return mData.Width;
    }

    int Window::GetHeight() const
    {
        return mData.Height;
    }

    void Window::UpdateSize(int width, int height)
    {
        UpdateAR(width, height);
        SetSize(width, height);
        glfwSetWindowSize(mWindow, width, height);
    }

    void Window::SetAR(float aspectRatio)
    {
        mData.AR = aspectRatio;
    }

    void Window::UpdateAR(int width, int height)
    {
        Root::GetWindow()->SetAR(static_cast<float>(width) / static_cast<float>(height));
    }

    bool Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mWindow = glfwCreateWindow(mData.Width, mData.Height, mData.Title, nullptr, nullptr);

        if (mWindow == nullptr)
            return false;

        glfwMakeContextCurrent(mWindow);

        glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow*, int width, int height) {
            Event::Dispatcher::Post(Event::FrameBufferSizeEvent(width, height));
        });
        glfwSetKeyCallback(mWindow, [](GLFWwindow*, int key, int, int action, int) {
            Event::Dispatcher::Post(Event::KeyInputEvent(key, action));
        });
        glfwSetMouseButtonCallback(mWindow, [](GLFWwindow*, int button, int action, int) {
            Event::Dispatcher::Post(Event::MouseButtonEvent(button, action));
        });

        glfwSetWindowFocusCallback(mWindow, [](GLFWwindow*, int isFocused) {
            Event::Dispatcher::Post(Event::FocusEvent(isFocused));
        });

        UpdateAR(mData.Width, mData.Height);

        return true;
    }
}
