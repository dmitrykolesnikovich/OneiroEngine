//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Window.hpp"

#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Renderer/Vulkan/WindowSurface.hpp"
#include "Oneiro/Runtime/Application.hpp"

namespace oe::Core
{
	Window::~Window() { glfwDestroyWindow(mWindow); }

	void Window::SwapBuffers() const { /*glfwSwapBuffers(mWindow);*/ }

	void Window::PollEvents() { glfwPollEvents(); }

	void Window::WaitEvents() { glfwWaitEvents(); }

	bool Window::IsClosed() const { return glfwWindowShouldClose(mWindow) != 0; }

	const Window::WindowData& Window::GetData() const { return mData; }

	GLFWwindow* Window::GetGLFW() const { return mWindow; }

	void Window::SetWidth(int width) const { UpdateSize(width, mData.height); }

	void Window::SetHeight(int height) const { UpdateSize(mData.width, height); }

	void Window::SetSize(int width, int height) { mData.width = width; mData.height = height; }

	void Window::SetFramerate(int fps) { /*glfwSwapInterval(fps);*/ }

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
		//std::string glVersion = Root::GetGLVersion();
        /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::atoi(&glVersion[0]));
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::atoi(&glVersion[2]));*/
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        /*glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

        mWindow = glfwCreateWindow(mData.width, mData.height, mData.title, nullptr, nullptr);

        if (mWindow == nullptr)
            return false;

        //glfwMakeContextCurrent(mWindow);
        glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow*, int width, int height)
	        {
		        Event::Dispatcher::Post(Event::FrameBufferSizeEvent(width, height));
	        });
        glfwSetKeyCallback(mWindow, [](GLFWwindow*, int key, int, int action, int)
	        {
				Event::Dispatcher::Post(Event::KeyInputEvent(key, action));
	        });
		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow*, int button, int action, int)
			{
				Event::Dispatcher::Post(Event::MouseButtonEvent(button, action));
			});

		glfwSetWindowFocusCallback(mWindow, [](GLFWwindow*, int isFocused)
			{
				Event::Dispatcher::Post(Event::FocusEvent(isFocused));
			});

        UpdateAR(mData.width, mData.height);

        return true;
    }
}
