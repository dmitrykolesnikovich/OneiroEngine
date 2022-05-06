//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/MSAA.hpp"
#include "Oneiro/Renderer/Vulkan/Texture.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    void MSAA::Create()
    {
        const VkFormat colorFormat = GetSwapChain()->GetImageFormat();
        Image::Create(GetSwapChain()->GetExtent2D().width, GetSwapChain()->GetExtent2D().height, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mImage, mDeviceMemory, GetPhysDevice()->GetMsaaSamples());
        Image::CreateView(&mView, mImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void MSAA::Destroy()
    {
        const auto logicalDevice = GetLogicalDevice();
        logicalDevice->WaitIdle();
        vkDestroyImageView(logicalDevice->Get(), mView, nullptr);
        vkDestroyImage(logicalDevice->Get(), mImage, nullptr);
        vkFreeMemory(logicalDevice->Get(), mDeviceMemory, nullptr);
    }
}
