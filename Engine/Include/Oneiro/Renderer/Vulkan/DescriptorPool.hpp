#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class DescriptorPool
    {
    public:
        void Create(VkDescriptorType type);

        void Destroy();

        [[nodiscard]] VkDescriptorPool Get() const;
    private:
        VkDescriptorPool mDescriptorPool{};
    };
}
