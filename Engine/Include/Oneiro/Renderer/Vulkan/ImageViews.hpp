//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    class ImageViews
    {
    public:
        void Create(const SwapChain* swapChain);

        void Destroy();

        [[nodiscard]] const std::vector<VkImageView>& Get() const;
    private:
        void CreateImageView(VkImage swapChainImage, VkImageViewCreateInfo& createInfo) const;
        std::vector<VkImageView> mSwapChainImageViews;
    };
}
