//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Window.hpp"

namespace oe::Core
{
    bool Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mWindow = glfwCreateWindow(mData.width, mData.height, mData.title, nullptr, nullptr);

        if (mWindow == nullptr)
            return false;

        glfwMakeContextCurrent(mWindow);

        UpdateAR(mData.width, mData.height);

        return true;
    }

    void Window::SetKeyCallback(Callbacks::keyCallback kCallback)
    {
        mCallbacks.key = kCallback;
        glfwSetKeyCallback(Root::GetWindow()->GetGLFW(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            Window::mCallbacks.key(static_cast<Input::Key>(key), static_cast<Input::Action>(action));
        });
    }

    void Window::SetMouseButtonCallback(Callbacks::mouseButtonCallback mbCallback)
    {
        mCallbacks.mouseButton = mbCallback;
        glfwSetMouseButtonCallback(Root::GetWindow()->GetGLFW(), [](GLFWwindow*, int button, int action, int)
        {
            Window::mCallbacks.mouseButton(static_cast<Input::Button>(button), static_cast<Input::Action>(action));
        });
    }

    void Window::SetFrameBufferSizeCallback(Callbacks::frameBufferSizeCallback fbsCallback)
    {
        mCallbacks.frameBufferSize = fbsCallback;
        glfwSetFramebufferSizeCallback(Root::GetWindow()->GetGLFW(),[](GLFWwindow*, int w, int h)
        {
            Window::mCallbacks.frameBufferSize(w, h);
            Window::UpdateSize(w, h);
        });
    }

    void Window::SetFocusCallback(Callbacks::focusCallback focusCallback)
    {
        mCallbacks.focus = focusCallback;
        glfwSetWindowFocusCallback(Root::GetWindow()->GetGLFW(), [](GLFWwindow*, int isFocused){
            mCallbacks.focus(isFocused);
        });
    }

    void Window::SetErrorCallback(Callbacks::errorCallback errorCallback)
    {
        mCallbacks.error = errorCallback;
        glfwSetErrorCallback([](int code, const char* description){
            mCallbacks.error(code, description);
        });
    }

    Window::WindowCallbacks Window::mCallbacks{};
}