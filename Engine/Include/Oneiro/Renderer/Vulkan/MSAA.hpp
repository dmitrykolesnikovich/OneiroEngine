//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class MSAA
    {
      public:
        void Create();

        [[nodiscard]] VkImageView GetView() const
        {
            return mView;
        };

        void Destroy();

      private:
        VkImage mImage{};
        VkDeviceMemory mDeviceMemory{};
        VkImageView mView{};
    };
} // namespace oe::Renderer::Vulkan
