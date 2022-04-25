//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>

#include "Oneiro/Core/Oneiro.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "vulkan/vulkan_core.h"

namespace oe::Core
{
	class Window;
	class Config;
}

namespace oe::Runtime
{
	class Engine;
	class Application;
}

namespace oe::Renderer::Vulkan
{
	class Instance;
	class PhysicalDevice;
	class LogicalDevice;
	class WindowSurface;
	class SwapChain;
	class ImageViews;
	class Pipeline;
	class RenderPass;
	class Framebuffer;
	class CommandPool;
	class CommandBuffer;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	class DescriptorSetLayout;
	class UniformBuffer;
	class DescriptorPool;
	class DescriptorSet;

	void PreInit();
	void Init();
	void Shutdown();

	void ReCreateSwapChain();

	void BeginScene();
	void EndScene();

	void AddVertexShader(VkShaderModule shader);
	void AddFragmentShader(VkShaderModule shader);

	void UpdateCurrentImageIndex();

	const Instance* GetInstance();
	const PhysicalDevice* GetPhysDevice();
	const std::vector<const char*>& GetValidationLayers();
	const WindowSurface* GetWindowSurface();
	const LogicalDevice* GetLogicalDevice();
	const SwapChain* GetSwapChain();
	const ImageViews* GetImageViews();
	const RenderPass* GetRenderPass();
	const Pipeline* GetPipeline();
	const std::vector<Framebuffer>& GetFramebuffers();
	const CommandPool* GetCommandPool();
	const CommandBuffer* GetCommandBuffer();
	const DescriptorPool* GetDescriptorPool();
	const DescriptorSet* GetDescriptorSet();
	VkQueue* GetGraphicsQueuePtr();
	VkQueue GetGraphicsQueue();
	VkQueue GetPresentQueue();
	VkQueue* GetPresentQueuePtr();
	VkSemaphore GetImageAvaibleSemaphores();
	VkSemaphore GetRenderFinishedSemaphores();
	VkFence GetInFlightFence();
	uint32_t GetCurrentImageIndex();

	std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions();
	std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions();
	std::vector<DescriptorSetLayout>& GetDescriptorSetLayouts();
}
