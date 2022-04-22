#include "Oneiro/Renderer/Vulkan/VertexBuffer.hpp"

namespace oe::Renderer::Vulkan
{
    void VertexBuffer::Bind(VkCommandBuffer commandBuffer) const
    {
        const VkBuffer buffers[] = { mBuffer };
        constexpr VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void VertexBuffer::Destroy()
    {
        vkDeviceWaitIdle(Core::Root::Vulkan::GetLogicalDevice()->Get());
        vkDestroyBuffer(Core::Root::Vulkan::GetLogicalDevice()->Get(), mBuffer, nullptr);
        vkFreeMemory(Core::Root::Vulkan::GetLogicalDevice()->Get(), mBufferMemory, nullptr);
    }
}