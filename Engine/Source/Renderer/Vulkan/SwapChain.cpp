#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include <algorithm>
#include <stdexcept>

#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    void SwapChain::Create()
    {
        const auto& physDevice = Core::Root::Vulkan::GetPhysDevice()->Get();
        const auto& capabilities = PhysicalDevice::GetSurfaceCapabilities(physDevice);
        const auto& presentModes = PhysicalDevice::GetPresentModes(physDevice);
        const auto& surfaceFormats = PhysicalDevice::GetSurfaceFormats(physDevice);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(surfaceFormats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);
        VkExtent2D extent = ChooseSwapExtent(capabilities);
        uint32_t imageCount = capabilities.minImageCount + 1;

        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = Core::Root::Vulkan::GetWindowSurface()->Get();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        const PhysicalDevice::QueueFamilyIndices indices = PhysicalDevice::FindQueueFamilies(physDevice);
        const uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

        if (indices.GraphicsFamily != indices.PresentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(Core::Root::Vulkan::GetLogicalDevice()->Get(), &createInfo,
            nullptr, &mSwapchain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }
        vkGetSwapchainImagesKHR(Core::Root::Vulkan::GetLogicalDevice()->Get(), mSwapchain,
            &imageCount, nullptr);
        mSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(Core::Root::Vulkan::GetLogicalDevice()->Get(), mSwapchain,
            &imageCount, mSwapChainImages.data());
        mSwapChainImageFormat = surfaceFormat.format;
        mSwapChainExtent = extent;
    }

    void SwapChain::Destroy()
    {
        vkDestroySwapchainKHR(Core::Root::Vulkan::GetLogicalDevice()->Get(), mSwapchain, nullptr);
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
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        if (capabilities.currentExtent.width != UINT_MAX) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
}