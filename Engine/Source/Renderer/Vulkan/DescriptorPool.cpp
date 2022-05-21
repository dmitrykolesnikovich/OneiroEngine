//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

#include "imgui.h"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    void DescriptorPool::Create(VkDescriptorType type, uint32_t bindingsCount)
    {
        VkDescriptorPoolSize poolSize;
        poolSize.type = type;
        poolSize.descriptorCount = bindingsCount * bindingsCount;
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = bindingsCount * bindingsCount;

        VK_CHECK_RESULT(vkCreateDescriptorPool(GetLogicalDevice()->Get(), &poolInfo, nullptr,
                                               &mDescriptorPool),
                        "Failed to create descriptor pool!")
    }

    void DescriptorPool::Create()
    {
        constexpr VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000}, {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        poolInfo.poolSizeCount = (uint32_t) IM_ARRAYSIZE(pool_sizes);
        poolInfo.pPoolSizes = pool_sizes;

        VK_CHECK_RESULT(vkCreateDescriptorPool(GetLogicalDevice()->Get(), &poolInfo, nullptr,
                                               &mDescriptorPool),
                        "Failed to create descriptor pool!")
    }

    void DescriptorPool::Destroy()
    {
        vkDestroyDescriptorPool(GetLogicalDevice()->Get(), mDescriptorPool, nullptr);
    }

    VkDescriptorPool DescriptorPool::Get() const
    {
        return mDescriptorPool;
    }
}
