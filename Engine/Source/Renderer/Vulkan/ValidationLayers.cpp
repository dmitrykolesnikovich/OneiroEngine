#include "Oneiro/Renderer/Vulkan/ValidationLayers.hpp"

#include <iostream>

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Root.hpp"

namespace
{
    VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        std::cerr << pCallbackData->pMessage << '\n';

        return VK_TRUE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        if (const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")); func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator) {
        if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")); func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }
}

#include "Oneiro/Renderer/Vulkan/Intance.hpp"

namespace oe::Renderer::Vulkan
{
    void ValidationLayers::Destroy() const
    {
        if (mDebugUtilsMessenger != VK_NULL_HANDLE)
            DestroyDebugUtilsMessengerEXT(Core::Root::Vulkan::GetInstance()->Get(),
                mDebugUtilsMessenger, nullptr);
    }

    bool ValidationLayers::CheckSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : Core::Root::Vulkan::GetValidationLayers()) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void ValidationLayers::PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
    }

    void ValidationLayers::Setup()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(Core::Root::Vulkan::GetInstance()->Get(), &createInfo, nullptr, &mDebugUtilsMessenger) != VK_SUCCESS) {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }
}