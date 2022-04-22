#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Buffer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    void UniformBuffer::Create(VkDeviceSize size)
    {
        Buffer::Create(Core::Root::Vulkan::GetLogicalDevice()->Get(), size, 
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,mBuffer, mBufferMemory);
    }

    VkBuffer UniformBuffer::Get() const { return mBuffer; }

    void UniformBuffer::Destroy()
    {
        vkDestroyBuffer(Core::Root::Vulkan::GetLogicalDevice()->Get(), mBuffer, nullptr);
        vkFreeMemory(Core::Root::Vulkan::GetLogicalDevice()->Get(), mBufferMemory, nullptr);
    }
}