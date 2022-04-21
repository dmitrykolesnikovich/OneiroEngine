#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class WindowSurface
    {
    public:
        void Destroy() const;

        [[nodiscard]] VkSurfaceKHR Get() const;
        [[nodiscard]] const VkSurfaceKHR* GetPtr() const;

        void Setup(GLFWwindow* window);
    private:
        VkSurfaceKHR mSurface{};
    };
}
