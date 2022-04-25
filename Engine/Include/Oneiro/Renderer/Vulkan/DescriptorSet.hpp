//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
	class DescriptorSet
	{
	public:
		void Begin(VkDescriptorPool pool, const VkDescriptorSetLayout* layout);
		void AddBufferWriteDescriptor(int binding, VkBuffer buffer, VkDeviceSize bufferSize);
		void AddImageWriteDescriptor(int binding, VkImageView imageView, VkSampler imageSampler);
		void End();

		void Bind() const;
	private:
		std::vector<VkWriteDescriptorSet> mDescriptorWrites;
		VkDescriptorSet mDescriptorSet{};
	};
}
