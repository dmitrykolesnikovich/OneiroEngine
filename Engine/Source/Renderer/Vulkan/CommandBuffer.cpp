//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	void CommandBuffer::Create()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = GetCommandPool()->Get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		// TODO: multi-threaded command buffer generation
		VK_CHECK_RESULT(vkAllocateCommandBuffers(GetLogicalDevice()->Get(),
			                &allocInfo, &mCommandBuffer), "Failed to allocate command buffers!")
	}

	void CommandBuffer::Begin() const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		VK_CHECK_RESULT(vkBeginCommandBuffer(mCommandBuffer, &beginInfo), "Failed to begin recording command buffer!")
	}

	void CommandBuffer::End() const
	{
		VK_CHECK_RESULT(vkEndCommandBuffer(mCommandBuffer), "Failed to record command buffer!")
	}

	void CommandBuffer::Reset() const { vkResetCommandBuffer(mCommandBuffer, {}); }

	VkCommandBuffer CommandBuffer::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GetCommandPool()->Get();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(GetLogicalDevice()->Get(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void CommandBuffer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		const auto graphicsQueue = GetGraphicsQueue();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(GetLogicalDevice()->Get(), GetCommandPool()->Get(), 1, &commandBuffer);
	}

	VkCommandBuffer CommandBuffer::Get() const { return mCommandBuffer; }
	const VkCommandBuffer* CommandBuffer::GetPtr() const { return &mCommandBuffer; }
}
