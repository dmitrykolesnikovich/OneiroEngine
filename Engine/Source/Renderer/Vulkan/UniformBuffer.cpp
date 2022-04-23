//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/Buffer.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    void UniformBuffer::Create(int binding, VkShaderStageFlagBits stage, VkDeviceSize size)
    {
        Buffer::Create(GetLogicalDevice()->Get(), size, 
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,mBuffer, mBufferMemory);
        DescriptorSetLayout layout;
        mDescriptorPool.Create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        layout.Create(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stage);
        GetDescriptorSetLayouts().push_back(layout);
        mDescriptorSet.Create(mBuffer, binding, size, layout.GetPtr(), mDescriptorPool.Get());
    }

    VkBuffer UniformBuffer::Get() const { return mBuffer; }

    void UniformBuffer::Destroy()
    {
        vkDeviceWaitIdle(GetLogicalDevice()->Get());
        mDescriptorPool.Destroy();
        vkDestroyBuffer(GetLogicalDevice()->Get(), mBuffer, nullptr);
        vkFreeMemory(GetLogicalDevice()->Get(), mBufferMemory, nullptr);
    }
}