//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"

#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/DepthBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"
#include "Oneiro/Renderer/Vulkan/ImageViews.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/MSAA.hpp"
#include "Oneiro/Renderer/vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <stdexcept>

namespace
{
	std::vector<VkShaderModule> _vertShaders;
	std::vector<VkShaderModule> _fragShaders;
	std::vector<oe::Renderer::Vulkan::DescriptorSetLayout> _descriptorSetLayouts;
	std::vector<VkVertexInputBindingDescription> _vertexInputBindingDescriptions{};
	std::vector<VkVertexInputAttributeDescription> _vertexInputAttributeDescriptions{};

	VkQueue _graphicsQueue{};
	VkQueue _presentQueue{};
	VkSemaphore _imageAvailableSemaphores{};
	VkSemaphore _renderFinishedSemaphores{};
	VkFence _inFlightFence{};

	oe::Renderer::Vulkan::Instance _instance;
	oe::Renderer::Vulkan::PhysicalDevice _physicalDevice;
	oe::Renderer::Vulkan::LogicalDevice _logicalDevice;
	oe::Renderer::Vulkan::WindowSurface _windowSurface;
	oe::Renderer::Vulkan::SwapChain _swapChain;
	oe::Renderer::Vulkan::ImageViews _imageViews;
	oe::Renderer::Vulkan::RenderPass _renderPass;
	oe::Renderer::Vulkan::Pipeline _pipeline;
	std::vector<oe::Renderer::Vulkan::Framebuffer> _framebuffers;
	oe::Renderer::Vulkan::CommandPool _commandPool;
	oe::Renderer::Vulkan::CommandBuffer _commandBuffer;
	oe::Renderer::Vulkan::DescriptorPool _descriptorPool;
	oe::Renderer::Vulkan::DescriptorSet _descriptorSet;
	oe::Renderer::Vulkan::DepthBuffer _depthBuffer;
	oe::Renderer::Vulkan::MSAA _msaa;

	std::vector _validationLayers = {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor"};
	uint32_t _currentImageIndex{};

	void CreateFramebuffers()
	{
		_framebuffers.resize(_imageViews.Get().size());
		const size_t size = _framebuffers.size();
		for (size_t i{}; i < size; ++i)
		{
			_framebuffers[i].Create({_msaa.GetView(), _depthBuffer.GetView(), _imageViews.Get()[i]});
		}
	}

	void DestroyFramebuffers()
	{
		for (auto& framebuffer : _framebuffers)
			framebuffer.Destroy();
	}

	void CreateAsyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		const auto& device = _logicalDevice.Get();

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &_inFlightFence) != VK_SUCCESS)
		{
			OE_THROW_ERROR("Renderer::Vulkan", "Failed to create sync objects for a frame!")
		}
	}

	void DesrtoyAsyncObjects()
	{
		const auto device = _logicalDevice.Get();
		vkDestroySemaphore(device, _imageAvailableSemaphores, nullptr);
		vkDestroySemaphore(device, _renderFinishedSemaphores, nullptr);
		vkDestroyFence(device, _inFlightFence, nullptr);
	}

	void DestroySwapChain()
	{
		vkDeviceWaitIdle(_logicalDevice.Get());
		DestroyFramebuffers();
		_pipeline.Destroy();
		_renderPass.Destroy();
		_msaa.Destroy();
		_depthBuffer.Destroy();
		_imageViews.Destroy();
		_swapChain.Destroy();
	}
}

namespace oe::Renderer::Vulkan
{
	void PreInit()
	{
		stbi_set_flip_vertically_on_load(1);
		_instance.Create();
		_windowSurface.Setup(Core::Root::GetWindow()->GetGLFW());
		_physicalDevice.Create();
		_logicalDevice.Create();
		_swapChain.Create();
		_renderPass.Create();
		_imageViews.Create();
		_commandPool.Create();
		_msaa.Create();
		_depthBuffer.Create();
		_descriptorPool.Create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		//GuiLayer::PreInit();
	}

	void Init()
	{
		_pipeline.Create(_vertShaders, _fragShaders);
		CreateFramebuffers();
		_commandBuffer.Create();
		CreateAsyncObjects();
		//GuiLayer::Init();
	}

	void Shutdown()
	{
		const auto device = _logicalDevice.Get();
		vkDeviceWaitIdle(device);
		for (const auto& fragShaderModule : _vertShaders)
			vkDestroyShaderModule(device, fragShaderModule, nullptr);
		for (const auto& vertShaderModule : _fragShaders)
			vkDestroyShaderModule(device, vertShaderModule, nullptr);
		DesrtoyAsyncObjects();
		for (auto& descriptorSetLayout : _descriptorSetLayouts)
			descriptorSetLayout.Destroy();
		_descriptorPool.Destroy();
		_commandPool.Destroy();
		DestroySwapChain();
		_logicalDevice.Destroy();
		_windowSurface.Destroy();
		_instance.Destroy();

		//GuiLayer::Shutdown();
	}

	void ReCreateSwapChain()
	{
		int width{}, height{};
		glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);
			glfwWaitEvents();
		}
		DestroySwapChain();

		_swapChain.Create();
		_imageViews.Create();
		_msaa.Create();
		_depthBuffer.Create();
		_renderPass.Create();
		_pipeline.Create(_vertShaders, _fragShaders);
		CreateFramebuffers();
	}

	void AddVertexShader(VkShaderModule shader)
	{
		_vertShaders.push_back(shader);
	}

	void AddFragmentShader(VkShaderModule shader)
	{
		_fragShaders.push_back(shader);
	}

	void UpdateCurrentImageIndex()
	{
		const VkResult result = vkAcquireNextImageKHR(_logicalDevice.Get(), _swapChain.Get(),
		                                              UINT64_MAX, _imageAvailableSemaphores, VK_NULL_HANDLE,
		                                              &_currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			ReCreateSwapChain();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			OE_THROW_ERROR("Renderer::Vulkan", "Failed to acquire swapchain image!");
		}
	}

	const Instance* GetInstance() { return &_instance; }

	const PhysicalDevice* GetPhysDevice() { return &_physicalDevice; }

	const std::vector<const char*>& GetValidationLayers() { return _validationLayers; }

	const WindowSurface* GetWindowSurface() { return &_windowSurface; }

	const LogicalDevice* GetLogicalDevice() { return &_logicalDevice; }

	const SwapChain* GetSwapChain() { return &_swapChain; }

	const ImageViews* GetImageViews() { return &_imageViews; }

	const RenderPass* GetRenderPass() { return &_renderPass; }

	const Pipeline* GetPipeline() { return &_pipeline; }

	const std::vector<Framebuffer>& GetFramebuffers() { return _framebuffers; }

	const CommandPool* GetCommandPool() { return &_commandPool; }

	const CommandBuffer* GetCommandBuffer() { return &_commandBuffer; }

	const DescriptorPool* GetDescriptorPool() { return &_descriptorPool; }

	const DescriptorSet* GetDescriptorSet() { return &_descriptorSet; }

	VkQueue* GetGraphicsQueuePtr() { return &_graphicsQueue; }

	VkQueue GetGraphicsQueue() { return _graphicsQueue; }

	VkQueue GetPresentQueue() { return _presentQueue; }

	VkQueue* GetPresentQueuePtr() { return &_presentQueue; }

	VkSemaphore GetImageAvaibleSemaphores() { return _imageAvailableSemaphores; }

	VkSemaphore GetRenderFinishedSemaphores() { return _renderFinishedSemaphores; }

	VkFence GetInFlightFence() { return _inFlightFence; }

	uint32_t GetCurrentImageIndex() { return _currentImageIndex; }

	std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions()
	{
		return _vertexInputBindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions()
	{
		return _vertexInputAttributeDescriptions;
	}

	std::vector<DescriptorSetLayout>& GetDescriptorSetLayouts()
	{
		return _descriptorSetLayouts;
	}

	void BeginScene()
	{
		const auto& commandBuffer = GetCommandBuffer();
		const auto& inFlightFence = GetInFlightFence();
		const auto& device = GetLogicalDevice()->Get();
		vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &inFlightFence);

		commandBuffer->Reset();
		commandBuffer->Begin();

		GetRenderPass()->Begin();
	}

	void EndScene()
	{
		_renderPass.End();

		_commandBuffer.End();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		const VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores};
		const VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = _commandBuffer.GetPtr();

		const VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFence) != VK_SUCCESS)
		{
			OE_THROW_ERROR("Renderer::Vulkan", "Failed to submit draw command buffer!")
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		const VkSwapchainKHR swapChains[] = {_swapChain.Get()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		const uint32_t imageIndex = GetCurrentImageIndex();
		presentInfo.pImageIndices = &imageIndex;

		VkResult result = vkQueuePresentKHR(GetPresentQueue(), &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			ReCreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			OE_THROW_ERROR("Renderer::Vulkan", "Failed to present swapchain image!")
		}
	}
}
