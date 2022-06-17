//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"

#ifndef OE_RENDERER_VULKAN

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

namespace oe::Renderer
{
	void PreInit()
	{
		stbi_set_flip_vertically_on_load(1);
	}

	void Init()
	{
		gl::Enable(gl::BLEND);
		gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
        gl::Enable(gl::DEPTH_TEST);
	}

	void Shutdown()
	{
	}

	void GL::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		gl::Viewport(x, y, width, height);
	}

	void GL::Viewport(GLsizei width, GLsizei height)
	{
		Viewport(0, 0, width, height);
	}
}
#else

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
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include "Oneiro/Renderer/Gui/GuiLayer.hpp"

#include <array>
#include <stdexcept>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

namespace
{
    std::vector<VkShaderModule> _vertShaders{};
    std::vector<VkShaderModule> _fragShaders{};
    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts{};
    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts3D{};
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
    std::vector<oe::Renderer::Vulkan::Framebuffer> _framebuffers;
    oe::Renderer::Vulkan::CommandPool _commandPool;
    oe::Renderer::Vulkan::CommandBuffer _commandBuffer;
    oe::Renderer::Vulkan::DescriptorSet _descriptorSet;
    oe::Renderer::Vulkan::DepthBuffer _depthBuffer;
    oe::Renderer::Vulkan::MSAA _msaa;

    oe::Renderer::Vulkan::DescriptorPool _guiDescriptorPool;

    oe::Renderer::Vulkan::Pipeline _2dPipeline;
    oe::Renderer::Vulkan::Pipeline _3dPipeline;

    oe::Renderer::Vulkan::Pipeline* _currentPipeline;
    std::vector<oe::Renderer::Vulkan::Pipeline*> _pipelines;

    const std::vector _validationLayers = {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor"};
    uint32_t _currentImageIndex{};
    uint32_t _guiCurrentImageIndex{};

    void CreateFramebuffers();
    void DestroyFramebuffers();
    void CreateAsyncObjects();
    void DesrtoyAsyncObjects();
    void DestroySwapChain(bool destroyShaders = false);
}

namespace oe::Renderer
{
    void PreInit()
    {
        _instance.Create();
        _windowSurface.Setup(Core::Root::GetWindow()->GetGLFW());
        _physicalDevice.Create();
        _logicalDevice.Create();
        _swapChain.Create();
        _renderPass.Create();
        _imageViews.Create(&_swapChain);
        _commandPool.Create();
        _msaa.Create();
        _depthBuffer.Create();
        _guiDescriptorPool.Create();

        GuiLayer::PreInit();
    }

    void Init()
    {
        //_2dPipeline.Create(VK_FRONT_FACE_CLOCKWISE,);
        //_3dPipeline.Create(VK_FRONT_FACE_COUNTER_CLOCKWISE);
        CreateFramebuffers();
        _commandBuffer.Create();
        CreateAsyncObjects();

        GuiLayer::Init();
    }

    void Shutdown()
    {
        _logicalDevice.WaitIdle();
        const auto device = _logicalDevice.Get();
        GuiLayer::Shutdown();
        _guiDescriptorPool.Destroy();
        DesrtoyAsyncObjects();
        DestroyDescriptorSetLayout();
        _commandPool.Destroy();
        DestroySwapChain(true);
        _logicalDevice.Destroy();
        _windowSurface.Destroy();
        _instance.Destroy();
    }
}

namespace oe::Renderer::Vulkan
{
    void DestroyShaders(VkDevice device)
    {
    }

    void DestroyDescriptorSetLayout()
    {
        const auto device = _logicalDevice.Get();
        for (const auto& descriptorSetLayout: _descriptorSetLayouts)
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        for (const auto& descriptorSetLayout: _descriptorSetLayouts3D)
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
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
        _imageViews.Create(&_swapChain);
        _renderPass.Create();
        _msaa.Create();
        _depthBuffer.Create();
        for (auto* pipeline : _pipelines)
        {
            pipeline->Create(VK_FRONT_FACE_CLOCKWISE, true);
        }
        CreateFramebuffers();
    }
    
    uint32_t UpdateCurrentImageIndex()
    {
        const VkResult result = vkAcquireNextImageKHR(_logicalDevice.Get(), _swapChain.Get(), UINT64_MAX, _imageAvailableSemaphores, VK_NULL_HANDLE, &_currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
            ReCreateSwapChain();
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        OE_THROW_ERROR("Renderer::Vulkan", "Failed to acquire swapchain image!");

        return _currentImageIndex;
    }

    VkDescriptorPool GetGuiDescriptorPool()
    {
        return _guiDescriptorPool.Get();
    }

    const Instance* GetInstance() { return &_instance; }

    const PhysicalDevice* GetPhysDevice() { return &_physicalDevice; }

    const std::vector<const char*>& GetValidationLayers() { return _validationLayers; }

    const WindowSurface* GetWindowSurface() { return &_windowSurface; }

    const LogicalDevice* GetLogicalDevice() { return &_logicalDevice; }

    const SwapChain* GetSwapChain() { return &_swapChain; }

    const ImageViews* GetImageViews() { return &_imageViews; }

    const RenderPass* GetRenderPass() { return &_renderPass; }

    const std::vector<Framebuffer>& GetFramebuffers() { return _framebuffers; }

    const CommandPool* GetCommandPool() { return &_commandPool; }

    const CommandBuffer* GetCommandBuffer() { return &_commandBuffer; }

    const DescriptorSet* GetDescriptorSet() { return &_descriptorSet; }

    VkQueue* GetGraphicsQueuePtr() { return &_graphicsQueue; }

    VkQueue GetGraphicsQueue() { return _graphicsQueue; }

    VkQueue GetPresentQueue() { return _presentQueue; }

    VkQueue* GetPresentQueuePtr() { return &_presentQueue; }

    VkSemaphore GetImageAvailableSemaphores() { return _imageAvailableSemaphores; }

    VkSemaphore GetRenderFinishedSemaphores() { return _renderFinishedSemaphores; }

    VkFence GetInFlightFence() { return _inFlightFence; }

    uint32_t GetCurrentImageIndex() { return _currentImageIndex; }
    uint32_t GetGuiCurrentImageIndex() { return _currentImageIndex; }

    Pipeline* GetCurrentPipeline() { return _currentPipeline; }

    std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions()
    {
        return _vertexInputBindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions()
    {
        return _vertexInputAttributeDescriptions;
    }

    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts()
    {
        return _descriptorSetLayouts;
    }

    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts3D()
    {
        return _descriptorSetLayouts3D;
    }

    void Bind2DPipeline()
    {
        _2dPipeline.Bind();
        _currentPipeline = &_2dPipeline;
    }

    void Bind3DPipeline()
    {
        _3dPipeline.Bind();
        _currentPipeline = &_3dPipeline;
    }

    void BeginScene()
    {
        const auto device = GetLogicalDevice()->Get();
        vkWaitForFences(device, 1, &_inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &_inFlightFence);

        _commandBuffer.Reset();
        _commandBuffer.Begin();
        RenderPass::Begin(_renderPass.Get());
    }

    void QueueSubmit(const VkSemaphore signalSemaphores[]);
    void QueuePresent(const VkSemaphore signalSemaphores[]);

    void EndScene()
    {
        _renderPass.End();

        _commandBuffer.End();
        QueueSubmit(&_renderFinishedSemaphores);
        QueuePresent(&_renderFinishedSemaphores);
    }


    void QueueSubmit(const VkSemaphore signalSemaphores[])
    {
        VkSubmitInfo submitInfo{};
        const VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores};
        constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = _commandBuffer.GetPtr();
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VK_CHECK_RESULT(vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFence), "Failed to submit draw command bufffer!")
    }

    void QueuePresent(const VkSemaphore signalSemaphores[])
    {
        const VkSwapchainKHR swapChains[] = {_swapChain.Get()};
        const uint32_t imageIndex = GetCurrentImageIndex();
        VkPresentInfoKHR presentInfo{};

        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        if (const VkResult result = vkQueuePresentKHR(GetPresentQueue(), &presentInfo);
                result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            ReCreateSwapChain();
        else if (result != VK_SUCCESS)
        OE_THROW_ERROR("Renderer::Vulkan", "Failed to present swapchain image!")
    }

    void Draw(uint32_t verticesCount)
    {
        vkCmdDraw(_commandBuffer.Get(), verticesCount, 1, 0, 0);
    }

    void DrawIndexed(uint32_t indexCount)
    {
        vkCmdDrawIndexed(_commandBuffer.Get(), indexCount, 1, 0, 0, 0);
    }
    void SetCurrentPipeline(Pipeline* pipeline)
    {
        _currentPipeline = pipeline;
    }
    void AddPipeline(Pipeline* pipeline)
    {
        _pipelines.push_back(pipeline);
    }
}

namespace
{
    void CreateFramebuffers()
    {
        _framebuffers.resize(_imageViews.Get().size());
        const size_t size = _framebuffers.size();
        for (size_t i{}; i < size; ++i)
            _framebuffers[i].Create({_msaa.GetView(), _depthBuffer.GetView(), _imageViews.Get()[i]});
    }
    

    void DestroyFramebuffers()
    {
        for (auto& framebuffer: _framebuffers)
            framebuffer.Destroy();
    }

    void CreateAsyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (const auto device = _logicalDevice.Get();
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores) != VK_SUCCESS || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores) != VK_SUCCESS || vkCreateFence(device, &fenceInfo, nullptr, &_inFlightFence) != VK_SUCCESS)
        OE_THROW_ERROR("Renderer::Vulkan", "Failed to create sync objects for a frame!")
    }

    void DesrtoyAsyncObjects()
    {
        const auto device = _logicalDevice.Get();
        vkDestroySemaphore(device, _imageAvailableSemaphores, nullptr);
        vkDestroySemaphore(device, _renderFinishedSemaphores, nullptr);
        vkDestroyFence(device, _inFlightFence, nullptr);
    }

    void DestroySwapChain(bool destroyShaders)
    {
        vkDeviceWaitIdle(_logicalDevice.Get());
        DestroyFramebuffers();
        for (auto* pipeline : _pipelines)
        {
            pipeline->Destroy(destroyShaders);
        }
        _renderPass.Destroy();
        _msaa.Destroy();
        _depthBuffer.Destroy();
        _imageViews.Destroy();
        _swapChain.Destroy();
    }
}
#endif
