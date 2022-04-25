#include "Oneiro/Renderer/Vulkan/Texture.hpp"
#include "Oneiro/Renderer/Vulkan/Buffer.hpp"

#include "stb/stb_image.h"

#include <stdexcept>

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/Vulkan/DepthBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(GetPhysDevice()->Get(), imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
			OE_THROW_ERROR("Renderer::Vulkan", "Texture image format does not support linear blitting!")

		const auto commandBuffer = CommandBuffer::BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		for (uint32_t i = 1; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
			                     VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			                     0, nullptr,
			                     0, nullptr,
			                     1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = {0, 0, 0};
			blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = {0, 0, 0};
			blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
			               image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			               image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			               1, &blit,
			               VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
			                     VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			                     0, nullptr,
			                     0, nullptr,
			                     1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
		                     VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		                     0, nullptr,
		                     0, nullptr,
		                     1, &barrier);

		CommandBuffer::EndSingleTimeCommands(commandBuffer);
	}

	void Texture::Load(const std::string& path)
	{
		const auto device = GetLogicalDevice()->Get();
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		const VkDeviceSize imageSize = texWidth * texHeight * 4;
		const uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

		if (!pixels)
			log::get("log")->warn("Failed to load texture at " + path + " path!");

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Buffer::Create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		               stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, imageSize);
		vkUnmapMemory(device, stagingBufferMemory);

		stbi_image_free(pixels);

		CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
		            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage, mDeviceMemory, VK_SAMPLE_COUNT_1_BIT, mipLevels);

		TransitionImageLayout(mImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
		                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
		Buffer::CopyToImage(stagingBuffer, mImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);

		CreateImageView(&mImageView, mImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
		CreateTextureSampler(&mSampler, mipLevels);
		GenerateMipmaps(mImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
	}

	void Texture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
	                                    VkImageLayout newLayout, uint32_t mipmaps)
	{
		const auto commandBuffer = CommandBuffer::BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipmaps;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout ==
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
		{
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		CommandBuffer::EndSingleTimeCommands(commandBuffer);
	}

	void Texture::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
	                          VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
	                          VkDeviceMemory& imageMemory, VkSampleCountFlagBits numSamples, uint32_t mipmaps)
	{
		const auto device = GetLogicalDevice()->Get();
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipmaps;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK_RESULT(vkCreateImage(device, &imageInfo, nullptr, &image), "Failed to create image!");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, properties);

		VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory), "Failed to allocate image memory!")

		vkBindImageMemory(device, image, imageMemory, 0);
	}

	void Texture::CreateImageView(VkImageView* imageView, VkImage image, VkFormat format,
	                              VkImageAspectFlags aspectFlags, uint32_t mipmaps)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipmaps;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		VK_CHECK_RESULT(vkCreateImageView(GetLogicalDevice()->Get(), &viewInfo, nullptr,
			                imageView), "Failed to create texture image view!")
	}

	void Texture::CreateTextureSampler(VkSampler* sampler, uint32_t mipmaps)
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(GetPhysDevice()->Get(), &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = static_cast<float>(mipmaps);
		samplerInfo.mipLodBias = 0.0f;

		VK_CHECK_RESULT(vkCreateSampler(GetLogicalDevice()->Get(), &samplerInfo,
			                nullptr, sampler), "Failed to create texture sampler!")
	}

	void Texture::Destroy()
	{
		const auto device = GetLogicalDevice()->Get();
		vkDeviceWaitIdle(device);
		vkDestroySampler(device, mSampler, nullptr);
		vkDestroyImageView(device, mImageView, nullptr);
		vkDestroyImage(device, mImage, nullptr);
		vkFreeMemory(device, mDeviceMemory, nullptr);
	}

	VkImage Texture::Get() const { return mImage; }

	VkImageView Texture::GetView() const { return mImageView; }

	VkSampler Texture::GetSampler() const { return mSampler; }
}
