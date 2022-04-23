//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    void DescriptorSetLayout::Create(int binding, VkDescriptorType type, VkShaderStageFlags stageFlags)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = binding;
        uboLayoutBinding.descriptorType = type;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = stageFlags;
        uboLayoutBinding.pImmutableSamplers = nullptr; 
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;
        if (vkCreateDescriptorSetLayout(GetLogicalDevice()->Get(), &layoutInfo, 
                                        nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    VkDescriptorSetLayout DescriptorSetLayout::Get() const
    {
        return mDescriptorSetLayout;
    }

    const VkDescriptorSetLayout* DescriptorSetLayout::GetPtr() const
    {
        return &mDescriptorSetLayout;
    }

    void DescriptorSetLayout::Destroy()
    {
        vkDestroyDescriptorSetLayout(GetLogicalDevice()->Get(),
                                     mDescriptorSetLayout, nullptr);
    }
}
