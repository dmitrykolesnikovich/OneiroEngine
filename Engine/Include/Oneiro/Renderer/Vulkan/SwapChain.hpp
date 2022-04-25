//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
	class SwapChain
	{
	public:
		void Create();

		void Destroy();

		[[nodiscard]] const std::vector<VkImage>& GetImages() const;
		[[nodiscard]] VkFormat GetImageFormat() const;
		[[nodiscard]] VkExtent2D GetExtent2D() const;
		[[nodiscard]] VkSwapchainKHR Get() const;
	private:
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		[[nodiscard]] VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
		VkSwapchainKHR mSwapchain{};
		std::vector<VkImage> mSwapChainImages{};
		VkFormat mSwapChainImageFormat{};
		VkExtent2D mSwapChainExtent{};
	};
}
