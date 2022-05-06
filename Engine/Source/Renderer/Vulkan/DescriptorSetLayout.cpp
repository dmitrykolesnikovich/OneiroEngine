//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

#include <stdexcept>

namespace oe::Renderer::Vulkan
{
    void DescriptorSetLayout::AddBinding(int binding, VkDescriptorType type, VkShaderStageFlagBits stage)
    {
        VkDescriptorSetLayoutBinding layout{};
        layout.binding = binding;
        layout.descriptorCount = 1;
        layout.descriptorType = type;
        layout.pImmutableSamplers = nullptr;
        layout.stageFlags = stage;

        mBindings.push_back(layout);
    }

    void DescriptorSetLayout::Create()
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = mBindings.size();
        layoutInfo.pBindings = mBindings.data();
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(GetLogicalDevice()->Get(), &layoutInfo, nullptr, &mDescriptorSetLayout), "Failed to create descriptor set layout!")
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
        vkDestroyDescriptorSetLayout(GetLogicalDevice()->Get(), mDescriptorSetLayout, nullptr);
    }
}
