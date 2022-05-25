//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/DepthBuffer.hpp"

#include <stdexcept>

#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Texture.hpp"

namespace oe::Renderer::Vulkan
{
    void DepthBuffer::Create()
    {
        const VkFormat depthFormat = FindDepthFormat();

        Image::Create(GetSwapChain()->GetExtent2D().width, GetSwapChain()->GetExtent2D().height,
                      depthFormat, VK_IMAGE_TILING_OPTIMAL,
                      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage, mImageMemory,
                      GetPhysDevice()->GetMsaaSamples());
        Image::CreateView(&mImageView, mImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    VkFormat DepthBuffer::FindDepthFormat()
    {
        return FindSupportedFormat(
                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    bool DepthBuffer::HasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    VkImageView DepthBuffer::GetView() const
    {
        return mImageView;
    }

    void DepthBuffer::Destroy()
    {
        const auto device = GetLogicalDevice()->Get();
        vkDeviceWaitIdle(device);
        vkDestroyImageView(device, mImageView, nullptr);
        vkDestroyImage(device, mImage, nullptr);
        vkFreeMemory(device, mImageMemory, nullptr);
    }

    VkFormat DepthBuffer::FindSupportedFormat(const std::vector<VkFormat>& candidates,
                                              VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        const auto physDevice = GetPhysDevice()->Get();
        for (const VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physDevice, format, &props);
            if (tiling == VK_IMAGE_TILING_LINEAR &&
                    (props.linearTilingFeatures & features) == features)
                return format;
            if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                    (props.optimalTilingFeatures & features) == features)
                return format;
        }

        OE_THROW_ERROR("Renderer::Vulkan", "Failed to find depth buffer supported format!")
    }
}
