#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class Buffer
    {
    public:
        static void Create(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
            VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        static void Copy(VkDevice device, VkCommandPool commandPool, VkBuffer srcBuffer,
                         VkBuffer dstBuffer, VkDeviceSize size, VkQueue graphicsQueue);
    private:
        static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}
