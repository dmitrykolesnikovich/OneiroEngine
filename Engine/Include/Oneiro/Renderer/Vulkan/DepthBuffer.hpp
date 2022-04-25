//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	class DepthBuffer
	{
	public:
		void Create();

		static VkFormat FindDepthFormat();

		static bool HasStencilComponent(VkFormat format);

		[[nodiscard]] VkImageView GetView() const;

		void Destroy();

	private:
		static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
		                                    VkFormatFeatureFlags features);

		VkImage mImage{};
		VkDeviceMemory mImageMemory{};
		VkImageView mImageView{};
	};
}
