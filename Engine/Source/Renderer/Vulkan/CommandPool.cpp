#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

namespace oe::Renderer::Vulkan
{
    void CommandPool::Create()
    {
        const PhysicalDevice::QueueFamilyIndices queueFamilyIndices = PhysicalDevice::FindQueueFamilies(
            Core::Root::Vulkan::GetPhysDevice()->Get());

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(Core::Root::Vulkan::GetLogicalDevice()->Get(), &poolInfo,
            nullptr, &mCommandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void CommandPool::Destroy()
    {
        vkDestroyCommandPool(Core::Root::Vulkan::GetLogicalDevice()->Get(), mCommandPool, nullptr);
    }

    VkCommandPool CommandPool::Get() const { return mCommandPool; }
    const VkCommandPool* CommandPool::GetPtr() const { return &mCommandPool; }
}