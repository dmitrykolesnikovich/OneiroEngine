//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/Intance.hpp"

#include <stdexcept>
#include <vector>

#include "Oneiro/Renderer/Renderer.hpp"

namespace
{
	std::vector<const char*> GetRequiredExtensions(bool debug)
	{
		uint32_t glfwExtensionCount{};
		const char** glfwExtensions{glfwGetRequiredInstanceExtensions(&glfwExtensionCount)};

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}
}

namespace oe::Renderer::Vulkan
{
	void Instance::Create(bool enableValidationLayers)
	{
		VkApplicationInfo applicationInfo{};
		VkInstanceCreateInfo createInfo{};
		const bool useValidationLayers{enableValidationLayers && ValidationLayers::CheckSupport()};
		const auto extensions = GetRequiredExtensions(enableValidationLayers);

		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = "Oneiro Engine";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Oneiro Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_3;

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &applicationInfo;

		if (useValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(GetValidationLayers().size());
			createInfo.ppEnabledLayerNames = GetValidationLayers().data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &mVkInstance), "Failed to create instance!")

		if (useValidationLayers)
			mValidationLayers.Setup();
	}

	const VkInstance* Instance::GetPtr() const
	{
		return &mVkInstance;
	}

	VkInstance Instance::Get() const
	{
		return mVkInstance;
	}

	void Instance::Destroy() const
	{
		mValidationLayers.Destroy();
		if (mVkInstance != VK_NULL_HANDLE)
			vkDestroyInstance(mVkInstance, nullptr);
	}
}
