//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>

#include "Buffer.hpp"
#include "LogicalDevice.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class VertexBuffer
    {
    public:
        template<class T>
        void Create(const std::vector<T>& vertices);
        void Bind() const;
        void Destroy();
    private:
        VkBuffer mBuffer{};
        VkMemoryRequirements mMemRequirements{};
        VkDeviceMemory mBufferMemory{};
    };

    template<class T>
    void VertexBuffer::Create(const std::vector<T>& vertices)
    {
        const VkDeviceSize size = sizeof(vertices[0]) * vertices.size();
        const auto device = GetLogicalDevice()->Get();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Buffer::Create(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
        memcpy(data, vertices.data(), size);
        vkUnmapMemory(device, stagingBufferMemory);
        Buffer::Create(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mBuffer, mBufferMemory);
        Buffer::Copy(stagingBuffer, mBuffer, size);
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }
}
