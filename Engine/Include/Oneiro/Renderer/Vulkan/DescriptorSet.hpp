#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class DescriptorSet
    {
    public:
        void Create();
        void Bind() const;
    private:
        VkDescriptorSet mDescriptorSet{};
    };
}
