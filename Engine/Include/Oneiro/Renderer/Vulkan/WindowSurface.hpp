//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

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
