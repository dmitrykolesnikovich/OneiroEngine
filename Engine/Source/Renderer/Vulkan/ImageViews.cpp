//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/ImageViews.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"

namespace oe::Renderer::Vulkan
{
	void ImageViews::Create()
	{
		const auto swapChainImages = GetSwapChain()->GetImages();
		mSwapChainImageViews.resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = GetSwapChain()->GetImageFormat();
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			VK_CHECK_RESULT(vkCreateImageView(GetLogicalDevice()->Get(), &createInfo,
				                nullptr, &mSwapChainImageViews[i]), "Failed to create image views!")
		}
	}

	void ImageViews::Destroy()
	{
		for (const auto imageView : mSwapChainImageViews)
		{
			vkDestroyImageView(GetLogicalDevice()->Get(), imageView, nullptr);
		}
	}

	const std::vector<VkImageView>& ImageViews::Get() const
	{
		return mSwapChainImageViews;
	}
}
