//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class DescriptorPool
    {
      public:
        void Create(VkDescriptorType type, uint32_t bindingsCount);
        void Create();

        void Destroy();

        [[nodiscard]] VkDescriptorPool Get() const;

      private:
        VkDescriptorPool mDescriptorPool{};
    };
} // namespace oe::Renderer::Vulkan
