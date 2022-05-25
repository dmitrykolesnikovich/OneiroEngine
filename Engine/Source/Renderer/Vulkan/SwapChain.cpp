//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"

#include <algorithm>
#include <stdexcept>

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"

namespace
{
    void CreateSwapChainInfo(uint32_t imageCount, VkSurfaceFormatKHR surfaceFormat,
                             VkSurfaceCapabilitiesKHR capabilities, VkPresentModeKHR presentMode,
                             VkExtent2D extent, VkSwapchainCreateInfoKHR& createInfo);
}

namespace oe::Renderer::Vulkan
{
    void SwapChain::Create()
    {
        const auto physDevice = GetPhysDevice()->Get();
        const auto& capabilities = PhysicalDevice::GetSurfaceCapabilities(physDevice);
        const auto& presentModes = PhysicalDevice::GetPresentModes(physDevice);
        const auto& surfaceFormats = PhysicalDevice::GetSurfaceFormats(physDevice);

        const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(surfaceFormats);
        const VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);
        const VkExtent2D extent = ChooseSwapExtent(capabilities);
        uint32_t imageCount = capabilities.minImageCount + 1;

        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
            imageCount = capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        CreateSwapChainInfo(imageCount, surfaceFormat, capabilities, presentMode, extent,
                            createInfo);

        VK_CHECK_RESULT(
                vkCreateSwapchainKHR(GetLogicalDevice()->Get(), &createInfo, nullptr, &mSwapchain),
                "Failed to create swapchain!")

        vkGetSwapchainImagesKHR(GetLogicalDevice()->Get(), mSwapchain, &imageCount, nullptr);
        mSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(GetLogicalDevice()->Get(), mSwapchain, &imageCount,
                                mSwapChainImages.data());
        mSwapChainImageFormat = surfaceFormat.format;
        mSwapChainExtent = extent;
    }

    void SwapChain::Destroy()
    {
        vkDestroySwapchainKHR(GetLogicalDevice()->Get(), mSwapchain, nullptr);
    }

    const std::vector<VkImage>& SwapChain::GetImages() const
    {
        return mSwapChainImages;
    }

    VkFormat SwapChain::GetImageFormat() const
    {
        return mSwapChainImageFormat;
    }

    VkExtent2D SwapChain::GetExtent2D() const
    {
        return mSwapChainExtent;
    }

    VkSwapchainKHR SwapChain::Get() const
    {
        return mSwapchain;
    }

    VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
            if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM &&
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;

        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::ChooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        if (capabilities.currentExtent.width != UINT_MAX)
            return capabilities.currentExtent;

        VkExtent2D actualExtent = {static_cast<uint32_t>(Core::Root::GetWindow()->GetWidth()),
                static_cast<uint32_t>(Core::Root::GetWindow()->GetHeight())};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                        capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                         capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

namespace
{
    void CreateSwapChainInfo(uint32_t imageCount, VkSurfaceFormatKHR surfaceFormat,
                             VkSurfaceCapabilitiesKHR capabilities, VkPresentModeKHR presentMode,
                             VkExtent2D extent, VkSwapchainCreateInfoKHR& createInfo)
    {
        using namespace oe::Renderer::Vulkan;

        const auto& physDevice = GetPhysDevice()->Get();
        const PhysicalDevice::QueueFamilyIndices
                indices = PhysicalDevice::FindQueueFamilies(physDevice);
        const uint32_t queueFamilyIndices[] =
                {indices.GraphicsFamily.value(), indices.PresentFamily.value()};

        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = GetWindowSurface()->Get();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (indices.GraphicsFamily != indices.PresentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
    }
}
