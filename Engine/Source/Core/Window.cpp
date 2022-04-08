//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Runtime/Application.hpp"

namespace oe::Core
{
	Window::~Window() { glfwDestroyWindow(mWindow); }

	void Window::SwapBuffers() const { glfwSwapBuffers(mWindow); }

	void Window::PollEvents() { glfwPollEvents(); }

	void Window::WaitEvents() { glfwWaitEvents(); }

	bool Window::IsClosed() const { return glfwWindowShouldClose(mWindow) != 0; }

	const Window::WindowData& Window::GetData() const { return mData; }

	GLFWwindow* Window::GetGLFW() const { return mWindow; }

	void Window::SetWidth(int width) const { UpdateSize(width, mData.height); }

	void Window::SetHeight(int height) const { UpdateSize(mData.width, height); }

	void Window::SetSize(int width, int height) { mData.width = width; mData.height = height; }

	void Window::SetFramerate(int fps) { glfwSwapInterval(fps); }

	void Window::UpdateSize(int width, int height)
	{
		UpdateAR(width, height);
		Root::GetWindow()->SetSize(width, height);
		glfwSetWindowSize(Root::GetWindow()->GetGLFW(), width, height);
	}

	void Window::SetAR(float aspectRatio) { mData.ar = aspectRatio; }

	void Window::UpdateAR(int width, int height) { Root::GetWindow()->SetAR(static_cast<float>(width) / static_cast<float>(height)); }

    bool Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
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
        if (kCallback)
        {
            mCallbacks.key = kCallback;
            glfwSetKeyCallback(Root::GetWindow()->GetGLFW(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
                {
                    if (Root::GetWindow()->IsKeyInput) 
                        mCallbacks.key(static_cast<Input::Key>(key), static_cast<Input::Action>(action));
                });
        }
        else
        {
            glfwSetKeyCallback(Root::GetWindow()->GetGLFW(), nullptr);
        }
    }

    void Window::SetMouseButtonCallback(Callbacks::mouseButtonCallback mbCallback)
    {
        if (mbCallback)
        {
            mCallbacks.mouseButton = mbCallback;
            glfwSetMouseButtonCallback(Root::GetWindow()->GetGLFW(), [](GLFWwindow*, int button, int action, int)
                {
                    if (Root::GetWindow()->IsMouseButtonInput) 
                        mCallbacks.mouseButton(static_cast<Input::Button>(button), static_cast<Input::Action>(action));
                });
        }
        else
        {
            glfwSetMouseButtonCallback(Root::GetWindow()->GetGLFW(), nullptr);
        }
    }

    void Window::SetFrameBufferSizeCallback(Callbacks::frameBufferSizeCallback fbsCallback)
    {
        mCallbacks.frameBufferSize = fbsCallback;
        glfwSetFramebufferSizeCallback(Root::GetWindow()->GetGLFW(),[](GLFWwindow*, int w, int h)
        {
	        mCallbacks.frameBufferSize(w, h);
	        UpdateSize(w, h);
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
