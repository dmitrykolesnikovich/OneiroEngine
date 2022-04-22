#include "Oneiro/Renderer/Vulkan/DescriptorSet.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include <stdexcept>

namespace oe::Renderer::Vulkan
{
    void DescriptorSet::Create()
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = Core::Root::Vulkan::GetDescriptorPool()->Get();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = Core::Root::Vulkan::GetDescriptorSetLayouts().data()->GetPtr();
        if (vkAllocateDescriptorSets(Core::Root::Vulkan::GetLogicalDevice()->Get(), &allocInfo, 
                                     &mDescriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = Core::Root::Vulkan::GetUniformBuffer()->Get();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = mDescriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; 
        descriptorWrite.pTexelBufferView = nullptr; 
        vkUpdateDescriptorSets(Core::Root::Vulkan::GetLogicalDevice()->Get(), 1, &descriptorWrite, 0, nullptr);
    }

    void DescriptorSet::Bind() const
    {
        vkCmdBindDescriptorSets(Core::Root::Vulkan::GetCommandBuffer()->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                Core::Root::Vulkan::GetPipeline()->GetLayout(), 0, 1, &mDescriptorSet, 0, nullptr);
    }
}