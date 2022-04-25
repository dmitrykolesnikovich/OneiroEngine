//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	void DescriptorPool::Create(VkDescriptorType type)
	{
		VkDescriptorPoolSize poolSize;
		poolSize.type = type;
		poolSize.descriptorCount = 200;
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = 200;

		VK_CHECK_RESULT(vkCreateDescriptorPool(GetLogicalDevice()->Get(),
			                &poolInfo, nullptr, &mDescriptorPool), "Failed to create descriptor pool!")
	}

	void DescriptorPool::Destroy()
	{
		vkDestroyDescriptorPool(GetLogicalDevice()->Get(), mDescriptorPool, nullptr);
	}

	VkDescriptorPool DescriptorPool::Get() const { return mDescriptorPool; }
}
