//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/VertexBuffer.hpp"

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"

namespace oe::Renderer::Vulkan
{
    void VertexBuffer::Bind() const
    {
        const VkBuffer buffers[] = {mBuffer};
        constexpr VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(GetCommandBuffer()->Get(), 0, 1, buffers, offsets);
    }

    void VertexBuffer::Destroy()
    {
        GetLogicalDevice()->WaitIdle();
        const auto device = GetLogicalDevice()->Get();
        vkDestroyBuffer(device, mBuffer, nullptr);
        vkFreeMemory(device, mBufferMemory, nullptr);
    }
}
