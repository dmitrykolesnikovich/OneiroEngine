//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/IndexBuffer.hpp"

namespace oe::Renderer::Vulkan
{
	void IndexBuffer::Bind(VkCommandBuffer commandBuffer) const
	{
		vkCmdBindIndexBuffer(commandBuffer, mBuffer, 0, VK_INDEX_TYPE_UINT16);
	}

	void IndexBuffer::Destroy()
	{
		vkDeviceWaitIdle(GetLogicalDevice()->Get());
		vkDestroyBuffer(GetLogicalDevice()->Get(), mBuffer, nullptr);
		vkFreeMemory(GetLogicalDevice()->Get(), mBufferMemory, nullptr);
	}
}
