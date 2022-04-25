//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/WindowSurface.hpp"

// TODO: Add support for Android/iOS
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "vulkan/vulkan_win32.h"
#elif __linux__
	#define GLFW_EXPOSE_NATIVE_WAYLAND
	#include "vulkan/vulkan_wayland.h"
	#include "GLFW/glfw3native.h"
#elif __APPLE__
	#include "vulkan/vulkan_macos.h"
#else
#error "Oneiro Engine supports only Windows, Linux and MacOS"
#endif

#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"

namespace oe::Renderer::Vulkan
{
	void WindowSurface::Destroy() const
	{
		vkDestroySurfaceKHR(GetInstance()->Get(), mSurface, nullptr);
	}

	VkSurfaceKHR WindowSurface::Get() const
	{
		return mSurface;
	}

	const VkSurfaceKHR* WindowSurface::GetPtr() const
	{
		return &mSurface;
	}
#ifdef _WIN32
	void WindowSurface::Setup(GLFWwindow* window)
	{
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(window);
		createInfo.hinstance = GetModuleHandle(nullptr);
		VK_CHECK_RESULT(vkCreateWin32SurfaceKHR(GetInstance()->Get(), &createInfo, nullptr,
			                &mSurface), "Failed to create window surface!")
	}
#elif __linux__
    void WindowSurface::Setup(GLFWwindow* window)
    {
        VkWaylandSurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.display = glfwGetWaylandDisplay();
        VK_CHECK_RESULT(vkCreateWaylandSurfaceKHR(GetInstance()->Get(), &createInfo, nullptr,
			&mSurface), "Failed to create window surface!")
    }
#elif __APPLE__
    void WindowSurface::Setup(GLFWwindow* window)
    {
        VkMacOSSurfaceCreateInfoMVK createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
		VK_CHECK_RESULT(vkCreateMacOSSurfaceMVK(GetInstance()->Get(), &createInfo, nullptr, 
			&mSurface), "Failed to create window surface!")
    }
#endif
}
