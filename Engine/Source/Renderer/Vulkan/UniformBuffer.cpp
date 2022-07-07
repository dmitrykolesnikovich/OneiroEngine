//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Buffer.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Texture.hpp"

namespace oe::Renderer::Vulkan
{
    void UniformBuffer::BeginBindings(VkDeviceSize size, uint32_t bindingsCount)
    {
        Buffer::Create(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       mBuffer, mBufferMemory);
        mDescriptorPool.Create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, bindingsCount);
        mDescriptorSet.Begin(mDescriptorPool.Get(), mSetLayout.GetPtr());
    }

    void UniformBuffer::AddBinding(int binding, VkDeviceSize size, VkShaderStageFlagBits stage, const Texture* texture)
    {
        if (texture)
        {
            mDescriptorSet.AddImageWriteDescriptor(binding, texture->GetView(), texture->GetSampler());
            return;
        }
        mDescriptorSet.AddBufferWriteDescriptor(binding, mBuffer, size);
    }

    void UniformBuffer::BeginLayouts() {}

    void UniformBuffer::AddLayout(int binding, VkDescriptorType type, VkShaderStageFlagBits stage)
    {
        mSetLayout.AddBinding(binding, type, stage);
    }

    void UniformBuffer::EndLayouts(bool is3d)
    {
        mSetLayout.Create();
        if (is3d)
            GetDescriptorSetLayouts3D().push_back(mSetLayout.Get());
        else
            GetDescriptorSetLayouts().push_back(mSetLayout.Get());
    }

    void UniformBuffer::EndBindings()
    {
        mDescriptorSet.End();
    }

    VkBuffer UniformBuffer::Get() const
    {
        return mBuffer;
    }

    void UniformBuffer::Destroy()
    {
        const auto device = GetLogicalDevice()->Get();
        vkDeviceWaitIdle(device);
        mDescriptorPool.Destroy();
        vkDestroyBuffer(device, mBuffer, nullptr);
        vkFreeMemory(device, mBufferMemory, nullptr);
    }
} // namespace oe::Renderer::Vulkan
