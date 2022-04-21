#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

#include <stdexcept>

#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Renderer::Vulkan
{
    void LogicalDevice::Destroy() { vkDestroyDevice(mDevice, nullptr); }
    VkDevice LogicalDevice::Get() const { return mDevice; }
    const VkDevice* LogicalDevice::GetPtr() const { return &mDevice; }

    void LogicalDevice::Create(bool enableValidationLayers)
    {
        const auto physicalDevice = Core::Root::Vulkan::GetPhysDevice();

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        auto& queueCreateInfos = Core::Root::Vulkan::GetPhysDevice()->GetQueueInfos();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = Core::Root::Vulkan::GetPhysDevice()->GetFeaturesPtr();

        createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice->GetExtensions().size());
        createInfo.ppEnabledExtensionNames = physicalDevice->GetExtensions().data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Core::Root::Vulkan::GetValidationLayers().size());
            createInfo.ppEnabledLayerNames = Core::Root::Vulkan::GetValidationLayers().data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice->Get(), &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }
        vkGetDeviceQueue(mDevice, Core::Root::Vulkan::GetPhysDevice()->GetQueueFamilyIndices().GraphicsFamily.value(),
            0, Core::Root::Vulkan::GetQueuePtr());
        vkGetDeviceQueue(mDevice, Core::Root::Vulkan::GetPhysDevice()->GetQueueFamilyIndices().PresentFamily.value(),
            0, Core::Root::Vulkan::GetPresentQueuePtr());
    }
}