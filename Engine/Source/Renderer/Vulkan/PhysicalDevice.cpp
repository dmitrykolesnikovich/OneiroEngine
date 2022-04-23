//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"

#include <map>
#include <set>
#include <stdexcept>
#include <string>

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"

namespace oe::Renderer::Vulkan
{
    bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device)
    {
        if (const QueueFamilyIndices indices = FindQueueFamilies(device); indices.IsComplete())
        {
            if (CheckDeviceExtensionsSupport(device))
            {
                const auto& formats = GetSurfaceFormats(device);
                if (const auto& presentModes = GetPresentModes(device);
                    !formats.empty() && !presentModes.empty())
                {
                    constexpr float queuePriority = 1.0f;
                    const std::set uniqueQueueFamilies = {
                        indices.GraphicsFamily.value(),
                        indices.PresentFamily.value()
                    };
                    for (const uint32_t queueFamily : uniqueQueueFamilies)
                    {
                        VkDeviceQueueCreateInfo queueCreateInfo{};
                        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                        queueCreateInfo.queueFamilyIndex = queueFamily;
                        queueCreateInfo.queueCount = 1;
                        queueCreateInfo.pQueuePriorities = &queuePriority;
                        mQueueCreateInfos.push_back(queueCreateInfo);
                    }
                    mQueueFamilyIndices = indices;
                    return true;
                }
            }
        }
        return false;
    }

    bool PhysicalDevice::CheckDeviceExtensionsSupport(VkPhysicalDevice device) const
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(mDeviceExtensions.cbegin(), mDeviceExtensions.cend());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    size_t PhysicalDevice::RateDeviceSuitability(VkPhysicalDevice device)
    {
        if (!IsDeviceSuitable(device))
            return -1;

        vkGetPhysicalDeviceProperties(device, &mDeviceProperties);
        vkGetPhysicalDeviceFeatures(device, &mDeviceFeatures);

        size_t score{};

        if (mDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 1000;

        score += mDeviceProperties.limits.maxImageDimension2D;

        if (!mDeviceFeatures.geometryShader) {
            return 0;
        }

        return score;
    }

    void PhysicalDevice::Create()
    {
        uint32_t deviceCount = 0;
        const auto instance = GetInstance()->Get();
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        std::multimap<size_t, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            size_t score = RateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }


        if (candidates.rbegin()->first > 0) {
            mPhysicalDevice = candidates.rbegin()->second;
        }
        else {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
    }

    bool PhysicalDevice::QueueFamilyIndices::IsComplete() const
    {
        return GraphicsFamily.has_value() && PresentFamily.has_value();
    }

    PhysicalDevice::QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, GetWindowSurface()->Get(), &presentSupport);
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
            }

            if (presentSupport) {
                indices.PresentFamily = i;
            }

            if (indices.IsComplete()) {
                break;
            }

            i++;
        }
        return indices;
    }

    VkPhysicalDevice PhysicalDevice::Get() const
    {
        return mPhysicalDevice;
    }

    const VkPhysicalDevice* PhysicalDevice::GetPtr() const
    {
        return &mPhysicalDevice;
    }

    VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const
    {
        return mDeviceFeatures;
    }

    const VkPhysicalDeviceFeatures* PhysicalDevice::GetFeaturesPtr() const
    {
        return &mDeviceFeatures;
    }

    const PhysicalDevice::QueueFamilyIndices& PhysicalDevice::GetQueueFamilyIndices() const
    {
        return mQueueFamilyIndices;
    }

    const std::vector<VkDeviceQueueCreateInfo>& PhysicalDevice::GetQueueInfos() const
    {
        return mQueueCreateInfos;
    }

    const std::vector<const char*>& PhysicalDevice::GetExtensions() const
    {
        return mDeviceExtensions;
    }

    VkSurfaceCapabilitiesKHR PhysicalDevice::GetSurfaceCapabilities(VkPhysicalDevice device)
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, GetWindowSurface()->Get(), &capabilities);
        return capabilities;
    }

    std::vector<VkSurfaceFormatKHR> PhysicalDevice::GetSurfaceFormats(VkPhysicalDevice device)
    {
        std::vector<VkSurfaceFormatKHR> formats;
        const auto surface = GetWindowSurface()->Get();
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());
        }

        return formats;
    }

    std::vector<VkPresentModeKHR> PhysicalDevice::GetPresentModes(VkPhysicalDevice device)
    {
        std::vector<VkPresentModeKHR> presentModes;
        const auto surface = GetWindowSurface()->Get();
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                &presentModeCount, presentModes.data());
        }

        return presentModes;
    }
}
