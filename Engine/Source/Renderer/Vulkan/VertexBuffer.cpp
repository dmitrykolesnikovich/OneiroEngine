//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/VertexBuffer.hpp"

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	void VertexBuffer::Bind(VkCommandBuffer commandBuffer) const
	{
		const VkBuffer buffers[] = {mBuffer};
		constexpr VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void VertexBuffer::Destroy()
	{
		const auto device = GetLogicalDevice()->Get();
		vkDeviceWaitIdle(device);
		vkDestroyBuffer(device, mBuffer, nullptr);
		vkFreeMemory(device, mBufferMemory, nullptr);
	}
}
