//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/IndexBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"

namespace oe::Renderer::Vulkan
{
    void IndexBuffer::Bind() const
    {
        vkCmdBindIndexBuffer(GetCommandBuffer()->Get(), mBuffer, 0, VK_INDEX_TYPE_UINT16);
    }

    void IndexBuffer::Destroy()
    {
        GetLogicalDevice()->WaitIdle();
        const auto device = GetLogicalDevice()->Get();
        vkDestroyBuffer(device, mBuffer, nullptr);
        vkFreeMemory(device, mBufferMemory, nullptr);
    }
}
