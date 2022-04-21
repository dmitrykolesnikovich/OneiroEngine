#include "Oneiro/Renderer/Vulkan/WindowSurface.hpp"

#include "Oneiro/Core/Root.hpp"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "vulkan/vulkan_win32.h"

#include <stdexcept>

#include "Oneiro/Renderer/Vulkan/Intance.hpp"

namespace oe::Renderer::Vulkan
{
    void WindowSurface::Destroy() const
    {
        vkDestroySurfaceKHR(Core::Root::Vulkan::GetInstance()->Get(), mSurface, nullptr);
    }

    VkSurfaceKHR WindowSurface::Get() const
    {
        return mSurface;
    }

    const VkSurfaceKHR* WindowSurface::GetPtr() const
    {
        return &mSurface;
    }

    void WindowSurface::Setup(GLFWwindow* window)
    {
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = glfwGetWin32Window(window);
        createInfo.hinstance = GetModuleHandle(nullptr);

        if (vkCreateWin32SurfaceKHR(Core::Root::Vulkan::GetInstance()->Get(), &createInfo, nullptr, &mSurface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }
        /*if (glfwCreateWindowSurface(Core::Root::Vulkan::GetInstance()->Get(), window, nullptr, &mSurface) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create window surface!");
            }*/
    }
}
