//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class LogicalDevice
    {
    public:
        void Destroy();

        [[nodiscard]] VkDevice Get() const;
        [[nodiscard]] const VkDevice* GetPtr() const;

        void Create(bool enableValidationLayers = true);
    private:
        VkDevice mDevice{};
    };
}