//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"

#include <stdexcept>

namespace oe::Renderer::Vulkan
{
    void LogicalDevice::Destroy()
    {
        vkDestroyDevice(mDevice, nullptr);
    }
    VkDevice LogicalDevice::Get() const
    {
        return mDevice;
    }
    const VkDevice* LogicalDevice::GetPtr() const
    {
        return &mDevice;
    }

    void LogicalDevice::WaitIdle() const
    {
        vkDeviceWaitIdle(mDevice);
    }

    void LogicalDevice::CreateDeviceInfo(bool enableValidationLayers, VkDeviceCreateInfo& createInfo) const
    {
        const auto physicalDevice = GetPhysDevice();
        auto& queueCreateInfos = physicalDevice->GetQueueInfos();

        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = GetPhysDevice()->GetFeaturesPtr();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice->GetExtensions().size());
        createInfo.ppEnabledExtensionNames = physicalDevice->GetExtensions().data();

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(GetValidationLayers().size());
            createInfo.ppEnabledLayerNames = GetValidationLayers().data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }
    }

    void LogicalDevice::Create(bool enableValidationLayers)
    {
        const auto physicalDevice = GetPhysDevice();

        VkDeviceCreateInfo createInfo{};
        CreateDeviceInfo(enableValidationLayers, createInfo);

        VK_CHECK_RESULT(vkCreateDevice(physicalDevice->Get(), &createInfo, nullptr, &mDevice), "Failed to create logical device!")

        vkGetDeviceQueue(mDevice, physicalDevice->GetQueueFamilyIndices().GraphicsFamily.value(), 0, GetGraphicsQueuePtr());
        vkGetDeviceQueue(mDevice, physicalDevice->GetQueueFamilyIndices().PresentFamily.value(), 0, GetPresentQueuePtr());
    }
} // namespace oe::Renderer::Vulkan
