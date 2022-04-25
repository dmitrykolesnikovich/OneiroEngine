//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	void CommandPool::Create()
	{
		const auto [GraphicsFamily, PresentFamily] = PhysicalDevice::FindQueueFamilies(
			GetPhysDevice()->Get());

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = GraphicsFamily.value();
		VK_CHECK_RESULT(vkCreateCommandPool(GetLogicalDevice()->Get(), &poolInfo,
			                nullptr, &mCommandPool), "Failed to create command pool!")
	}

	void CommandPool::Destroy()
	{
		vkDestroyCommandPool(GetLogicalDevice()->Get(), mCommandPool, nullptr);
	}

	VkCommandPool CommandPool::Get() const { return mCommandPool; }
	const VkCommandPool* CommandPool::GetPtr() const { return &mCommandPool; }
}
