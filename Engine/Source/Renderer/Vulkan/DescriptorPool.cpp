//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    void DescriptorPool::Create(VkDescriptorType type)
    {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = type;
        poolSize.descriptorCount = 1;
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(GetLogicalDevice()->Get(), 
                                   &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void DescriptorPool::Destroy()
    {
        vkDestroyDescriptorPool(GetLogicalDevice()->Get(), mDescriptorPool, nullptr);
    }

    VkDescriptorPool DescriptorPool::Get() const { return mDescriptorPool;}
}
