//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorSet.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    void DescriptorSet::Begin(VkDescriptorPool pool, const VkDescriptorSetLayout* layout)
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layout;
        VK_CHECK_RESULT(
                vkAllocateDescriptorSets(GetLogicalDevice()->Get(), &allocInfo, &mDescriptorSet),
                "Failed to allocate descriptor sets!")
    }

    void DescriptorSet::AddBufferWriteDescriptor(int binding, VkBuffer buffer,
                                                 VkDeviceSize bufferSize)
    {
        static VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;

        static VkWriteDescriptorSet descriptor;
        descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor.dstSet = mDescriptorSet;
        descriptor.dstBinding = binding;
        descriptor.dstArrayElement = 0;
        descriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor.descriptorCount = 1;
        descriptor.pBufferInfo = &bufferInfo;
        mDescriptorWrites.push_back(descriptor);
        vkUpdateDescriptorSets(GetLogicalDevice()->Get(), mDescriptorWrites.size(),
                               mDescriptorWrites.data(), 0, nullptr);
    }

    void DescriptorSet::AddImageWriteDescriptor(int binding, VkImageView imageView,
                                                VkSampler imageSampler)
    {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = imageView;
        imageInfo.sampler = imageSampler;

        static VkWriteDescriptorSet descriptor;
        descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor.dstSet = mDescriptorSet;
        descriptor.dstBinding = binding;
        descriptor.dstArrayElement = 0;
        descriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor.descriptorCount = 1;
        descriptor.pImageInfo = &imageInfo;
        mDescriptorWrites.push_back(descriptor);
        vkUpdateDescriptorSets(GetLogicalDevice()->Get(), mDescriptorWrites.size(),
                               mDescriptorWrites.data(), 0, nullptr);
    }

    void DescriptorSet::End()
    {
    }

    void DescriptorSet::Bind() const
    {
        vkCmdBindDescriptorSets(GetCommandBuffer()->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                                GetCurrentPipeline()->GetLayout(), 0, 1, &mDescriptorSet, 0,
                                nullptr);
    }
}
