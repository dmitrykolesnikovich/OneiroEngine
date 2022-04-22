#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Core/Root.hpp"
#include <stdexcept>

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

        if (vkCreateDescriptorPool(Core::Root::Vulkan::GetLogicalDevice()->Get(), 
                                   &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void DescriptorPool::Destroy()
    {
        vkDestroyDescriptorPool(Core::Root::Vulkan::GetLogicalDevice()->Get(), mDescriptorPool, nullptr);
    }

    VkDescriptorPool DescriptorPool::Get() const { return mDescriptorPool;}
}