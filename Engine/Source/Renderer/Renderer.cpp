//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/ImageViews.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>

#include "stb/stb_image.h"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"

namespace
{
    std::vector<VkShaderModule> mVertShaders;
    std::vector<VkShaderModule> mFragShaders;
    std::vector<oe::Renderer::Vulkan::DescriptorSetLayout> mDescriptorSetLayouts;
    std::vector<VkVertexInputBindingDescription> mVertexInputBindingDescriptions{};
    std::vector<VkVertexInputAttributeDescription> mVertexInputAttributeDescriptions{};

    VkQueue mGraphicsQueue{};
    VkQueue mPresentQueue{};
    VkSemaphore mImageAvailableSemaphores{};
    VkSemaphore mRenderFinishedSemaphores{};
    VkFence mInFlightFence{};

    oe::Renderer::Vulkan::Instance mInstance;
    oe::Renderer::Vulkan::PhysicalDevice mPhysicalDevice;
    oe::Renderer::Vulkan::LogicalDevice mLogicalDevice;
    oe::Renderer::Vulkan::WindowSurface mWindowSurface;
    oe::Renderer::Vulkan::SwapChain mSwapChain;
    oe::Renderer::Vulkan::ImageViews mImageViews;
    oe::Renderer::Vulkan::RenderPass mRenderPass;
    oe::Renderer::Vulkan::Pipeline mPipeline;
    std::vector<oe::Renderer::Vulkan::Framebuffer> mFramebuffers;
    oe::Renderer::Vulkan::CommandPool mCommandPool;
    oe::Renderer::Vulkan::CommandBuffer mCommandBuffer;
    oe::Renderer::Vulkan::DescriptorPool mDescriptorPool;
    oe::Renderer::Vulkan::DescriptorSet mDescriptorSet;
    std::vector mValidationLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor" };
    uint32_t mCurrentImageIndex{};

    void CreateFramebuffers()
    {
        mFramebuffers.resize(mImageViews.Get().size());
        int size = mFramebuffers.size();
        for (int i{}; i < size; ++i)
        {
            const VkImageView attachments[] = {
                mImageViews.Get()[i]
            };
            mFramebuffers[i].Create(attachments);
        }
    }

    void DestroyFramebuffers()
    {
        for (auto& framebuffer : mFramebuffers)
            framebuffer.Destroy();
    }

    void CreateAsyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        const auto& device = mLogicalDevice.Get();

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &mImageAvailableSemaphores) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &mRenderFinishedSemaphores) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &mInFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    void DesrtoyAsyncObjects()
    {
        const auto& device = mLogicalDevice.Get();
        vkDestroySemaphore(device, mImageAvailableSemaphores, nullptr);
        vkDestroySemaphore(device, mRenderFinishedSemaphores, nullptr);
        vkDestroyFence(device, mInFlightFence, nullptr);
    }

    void DestroySwapChain()
    {
        vkDeviceWaitIdle(mLogicalDevice.Get());
        DestroyFramebuffers();
        mPipeline.Destroy();
        mRenderPass.Destroy();
        mImageViews.Destroy();
        mSwapChain.Destroy();
    }
}

namespace oe::Renderer::Vulkan
{
    void PreInit()
    {
        stbi_set_flip_vertically_on_load(1);
        mInstance.Create();
        mWindowSurface.Setup(Core::Root::GetWindow()->GetGLFW());
        mPhysicalDevice.Create();
        mLogicalDevice.Create();
        mSwapChain.Create();
        mRenderPass.Create();
        mImageViews.Create();
        mCommandPool.Create();
	    mDescriptorPool.Create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        //GuiLayer::PreInit();
    }

    void Init()
    {
        mPipeline.Create(mVertShaders, mFragShaders);
        CreateFramebuffers();
        mCommandBuffer.Create();
        CreateAsyncObjects();
        //GuiLayer::Init();
    }

    void Shutdown()
    {
        const auto& device = mLogicalDevice.Get();
        vkDeviceWaitIdle(device);
        for (const auto& fragShaderModule : mVertShaders)
            vkDestroyShaderModule(device, fragShaderModule, nullptr);
        for (const auto& vertShaderModule : mFragShaders)
            vkDestroyShaderModule(device, vertShaderModule, nullptr);
        DesrtoyAsyncObjects();
        for (auto& descriptorSetLayout : mDescriptorSetLayouts)
            descriptorSetLayout.Destroy();
        mDescriptorPool.Destroy();
        mCommandPool.Destroy();
        DestroySwapChain();
        mLogicalDevice.Destroy();
        mWindowSurface.Destroy();
        mInstance.Destroy();

        //GuiLayer::Shutdown();
    }

    void ReCreateSwapChain()
    {
        int width{}, height{};
        glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);
            glfwWaitEvents();
        }
        DestroySwapChain();

        mSwapChain.Create();
        mImageViews.Create();
        mRenderPass.Create();
        mPipeline.Create(mVertShaders, mFragShaders);
        CreateFramebuffers();
    }

    void AddVertexShader(VkShaderModule shader)
    {
        mVertShaders.push_back(shader);
    }

    void AddFragmentShader(VkShaderModule shader)
    {
        mFragShaders.push_back(shader);
    }

    void UpdateCurrentImageIndex()
    {
        const VkResult result = vkAcquireNextImageKHR(mLogicalDevice.Get(), mSwapChain.Get(),
                                                      UINT64_MAX, mImageAvailableSemaphores, VK_NULL_HANDLE, &mCurrentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            ReCreateSwapChain();
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }
    }

    const Instance* GetInstance() { return &mInstance; }

    const PhysicalDevice* GetPhysDevice() { return &mPhysicalDevice; }

    const std::vector<const char*>& GetValidationLayers() { return mValidationLayers; }

    const WindowSurface* GetWindowSurface() { return &mWindowSurface;}

    const LogicalDevice* GetLogicalDevice() { return &mLogicalDevice; }

    const SwapChain* GetSwapChain() { return &mSwapChain; }

    const ImageViews* GetImageViews() { return &mImageViews; }

    const RenderPass* GetRenderPass() { return &mRenderPass; }

    const Pipeline* GetPipeline() { return &mPipeline; }

    const std::vector<Framebuffer>& GetFramebuffers() { return mFramebuffers; }

    const CommandPool* GetCommandPool() { return &mCommandPool; }

    const CommandBuffer* GetCommandBuffer() { return &mCommandBuffer; }

    const DescriptorPool* GetDescriptorPool() { return &mDescriptorPool; }

    const DescriptorSet* GetDescriptorSet() { return &mDescriptorSet; }

    VkQueue* GetGraphicsQueuePtr() { return &mGraphicsQueue; }

    VkQueue GetGraphicsQueue() { return mGraphicsQueue; }

    VkQueue GetPresentQueue() { return mPresentQueue; }

    VkQueue* GetPresentQueuePtr() { return &mPresentQueue; }

    VkSemaphore GetImageAvaibleSemaphores() { return mImageAvailableSemaphores; }

    VkSemaphore GetRenderFinishedSemaphores() { return mRenderFinishedSemaphores; }

    VkFence GetInFlightFence() { return mInFlightFence; }

    uint32_t GetCurrentImageIndex() { return mCurrentImageIndex; }

    std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions()
    {
        return mVertexInputBindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions()
    {
        return mVertexInputAttributeDescriptions;
    }

    std::vector<DescriptorSetLayout>& GetDescriptorSetLayouts()
    {
        return mDescriptorSetLayouts;
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
        mRenderPass.End();

        mCommandBuffer.End();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        const VkSemaphore waitSemaphores[] = { mImageAvailableSemaphores };
        const VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = mCommandBuffer.GetPtr();

        const VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphores };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mInFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        const VkSwapchainKHR swapChains[] = { mSwapChain.Get() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        const uint32_t imageIndex = GetCurrentImageIndex();
        presentInfo.pImageIndices = &imageIndex;

        VkResult result = vkQueuePresentKHR(GetPresentQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            ReCreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }
}
