#pragma once

#include <stdexcept>
#include <vector>

#include "Base.hpp"
#include "Buffer.hpp"
#include "CommandPool.hpp"
#include "LogicalDevice.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Renderer::Vulkan
{
    class VertexBuffer
    {
    public:
        template<class T>
        void Create(const std::vector<T>& vertices);
        void Bind(VkCommandBuffer commandBuffer) const;
        void Destroy();
    private:
        VkBuffer mBuffer{};
        VkMemoryRequirements mMemRequirements{};
        VkDeviceMemory mBufferMemory{};
    };

    template <class T>
    void VertexBuffer::Create(const std::vector<T>& vertices)
    {
        const VkDeviceSize size = sizeof(vertices[0]) * vertices.size();
        const auto device = Core::Root::Vulkan::GetLogicalDevice()->Get();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Buffer::Create(device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
        memcpy(data, vertices.data(), size);
        vkUnmapMemory(device, stagingBufferMemory);
        Buffer::Create(device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mBuffer, mBufferMemory);
        Buffer::Copy(device, Core::Root::Vulkan::GetCommandPool()->Get(),
            stagingBuffer, mBuffer, size, Core::Root::Vulkan::GetGraphicsQueue());
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }
}
