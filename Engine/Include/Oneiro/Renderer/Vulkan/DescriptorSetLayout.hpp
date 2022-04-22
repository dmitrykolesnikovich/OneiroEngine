#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class DescriptorSetLayout
    {
    public:
        void Create(int binding, VkDescriptorType type, VkShaderStageFlags stageFlags);

        [[nodiscard]] VkDescriptorSetLayout Get() const;

        [[nodiscard]] const VkDescriptorSetLayout* GetPtr() const;

        void Destroy();
    private:
        VkDescriptorSetLayout mDescriptorSetLayout{};
    };
}
