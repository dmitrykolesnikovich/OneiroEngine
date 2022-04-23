//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorSet.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    void DescriptorSet::Create(VkBuffer buffer, int binding, VkDeviceSize size, const VkDescriptorSetLayout* layout, VkDescriptorPool pool)
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layout;
        if (vkAllocateDescriptorSets(GetLogicalDevice()->Get(), &allocInfo, 
                                     &mDescriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = size;
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = mDescriptorSet;
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; 
        descriptorWrite.pTexelBufferView = nullptr; 
        vkUpdateDescriptorSets(GetLogicalDevice()->Get(), 1, &descriptorWrite, 0, nullptr);
    }

    void DescriptorSet::Bind() const
    {
        vkCmdBindDescriptorSets(GetCommandBuffer()->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                GetPipeline()->GetLayout(), 0, 1, &mDescriptorSet, 0, nullptr);
    }
}
