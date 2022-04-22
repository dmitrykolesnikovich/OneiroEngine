#include "Oneiro/Renderer/Vulkan/IndexBuffer.hpp"

namespace oe::Renderer::Vulkan
{
    void IndexBuffer::Bind(VkCommandBuffer commandBuffer) const
    {
        const VkBuffer buffers[] = { mBuffer };
        constexpr VkDeviceSize offsets[] = {0};
        vkCmdBindIndexBuffer(commandBuffer, mBuffer, 0, VK_INDEX_TYPE_UINT16);
    }

    void IndexBuffer::Destroy()
    {
        vkDeviceWaitIdle(Core::Root::Vulkan::GetLogicalDevice()->Get());
        vkDestroyBuffer(Core::Root::Vulkan::GetLogicalDevice()->Get(), mBuffer, nullptr);
        vkFreeMemory(Core::Root::Vulkan::GetLogicalDevice()->Get(), mBufferMemory, nullptr);
    }
}