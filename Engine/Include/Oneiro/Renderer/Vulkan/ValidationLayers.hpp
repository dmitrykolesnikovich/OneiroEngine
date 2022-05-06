//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class ValidationLayers
    {
    public:
        void Destroy() const;

        static bool CheckSupport();
        static void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        void Setup();
    private:
        VkDebugUtilsMessengerEXT mDebugUtilsMessenger{};
    };
}
