//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Buffer.hpp"
#include "LogicalDevice.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    class IndexBuffer
    {
      public:
        template <class T> void Create(const std::vector<T>& indices);
        void Bind() const;
        void Destroy();

      private:
        VkBuffer mBuffer{};
        VkDeviceMemory mBufferMemory{};
    };

    template <class T> void IndexBuffer::Create(const std::vector<T>& indices)
    {
        const VkDeviceSize size = sizeof(indices[0]) * indices.size();
        const auto device = GetLogicalDevice()->Get();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Buffer::Create(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       stagingBuffer, stagingBufferMemory);
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
        memcpy(data, indices.data(), size);
        vkUnmapMemory(device, stagingBufferMemory);
        Buffer::Create(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                       mBuffer, mBufferMemory);
        Buffer::Copy(stagingBuffer, mBuffer, size);
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }
} // namespace oe::Renderer::Vulkan
