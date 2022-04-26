//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <string>

namespace oe::Renderer::Vulkan
{
	class Texture
	{
	public:
		/**
		 * \brief Loading texture from path. \n
		 * WARNING: Load texture before creating uniform buffer.
		 * \param path path to texture
		 */
		void Load(const std::string& path);

		static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
		                                  VkImageLayout newLayout, uint32_t mipmaps = 1);

		static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		                        VkImageUsageFlags usage, VkMemoryPropertyFlags
		                        properties, VkImage& image, VkDeviceMemory& imageMemory,
		                        VkSampleCountFlagBits numSamples,
		                        uint32_t mipmaps = 1);

		static void CreateImageView(VkImageView* imageView, VkImage image, VkFormat format,
		                            VkImageAspectFlags aspectFlags, uint32_t mipmaps = 1);

		static void CreateTextureSampler(VkSampler* sampler, uint32_t mipmaps);

		void Destroy();

		[[nodiscard]] VkImage Get() const;
		[[nodiscard]] VkImageView GetView() const;
		[[nodiscard]] VkSampler GetSampler() const;
	private:
		VkImage mImage{};
		VkImageView mImageView{};
		VkSampler mSampler{};
		VkDeviceMemory mDeviceMemory{};
	};
}
