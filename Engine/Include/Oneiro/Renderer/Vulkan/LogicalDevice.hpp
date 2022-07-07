//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class LogicalDevice
    {
      public:
        void Create(bool enableValidationLayers = true);
        void Destroy();

        [[nodiscard]] VkDevice Get() const;
        [[nodiscard]] const VkDevice* GetPtr() const;

        void WaitIdle() const;

      private:
        void CreateDeviceInfo(bool enableValidationLayers, VkDeviceCreateInfo& createInfo) const;

        VkDevice mDevice{};
    };
} // namespace oe::Renderer::Vulkan
