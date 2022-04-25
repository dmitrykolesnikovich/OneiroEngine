//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	class Buffer
	{
	public:
		static void Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		                   VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		static void Copy(VkBuffer srcBuffer,
		                 VkBuffer dstBuffer, VkDeviceSize size);

		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static void CopyToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	};
}
