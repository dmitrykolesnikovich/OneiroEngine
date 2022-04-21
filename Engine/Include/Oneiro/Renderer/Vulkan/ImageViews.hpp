#pragma once

#include "Base.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    class ImageViews
    {
    public:
        void Create();

        void Destroy();

        [[nodiscard]] const std::vector<VkImageView>& Get() const;
    private:
        std::vector<VkImageView> swapChainImageViews;
    };
}
