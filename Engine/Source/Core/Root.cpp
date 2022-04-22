//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/ImageViews.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include "Oneiro/Renderer/Vulkan/VertexBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/IndexBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSet.hpp"

//#include <chrono>
//#include <future>

namespace oe::Core
{
	Root::Root() : mVulkan(new Vulkan)
	{
		/*mTextureManager = new ResourceManager<Renderer::Texture>;*/
        mConfigsMap["user"] = new Config("user.cfg");
        mConfigsMap["renderer"] = new Config("renderer.cfg");
	}

    Root::~Root()
    {
	    mWindowInstance = nullptr;
	    mApplicationInstance = nullptr;
    }

    Window* Root::GetWindow() { return mWindowInstance; }

    Runtime::Application* Root::GetApplication() { return mApplicationInstance; }

    /*ResourceManager<Renderer::Texture>* Root::GetTextureManager() { return mTextureManager; }

    std::vector<std::future<void>>& Root::GetFutures() { return mFutures; }*/

    //Config* Root::GetConfig(const std::string& name) { return mConfigsMap[name]; }

    //const std::string& Root::GetGLVersion() { return mGLVersion; }

    //const std::string& Root::GetGLSLVersion() { return mGLSLVersion; }

    //void Root::LoadTexturesAsync()
    //{
    //    using namespace std::chrono_literals;
    //    const auto& resources = mTextureManager->GetResources();
    //    for (const auto& resource : resources)
    //    {
    //        mFutures.emplace_back(std::async(std::launch::async,
    //            [](Renderer::Texture* texture) { texture->PreLoad(); }, resource.get()));
    //    }

    //    // TODO: optimize while loop
    //    const size_t futuresSize = mFutures.size();
    //    uint64_t i{0};
    //    while (i != futuresSize)
    //    {
    //        if (mFutures[i]._Is_ready())
    //        {
    //            mTextureManager->Get(i)->Load(); i++;
    //        }
    //    }
    //}

    void Root::SetApplication(Runtime::Application* app) { if (!mApplicationInstance) mApplicationInstance = app; }

    void Root::SetWindow(Window* window) { if (!mWindowInstance) mWindowInstance = window; }

    void Root::Vulkan::PreInit()
    {
        mInstance.Create();
        mWindowSurface.Setup(GetWindow()->GetGLFW());
        mPhysicalDevice.Create();
        mLogicalDevice.Create();
        mSwapChain.Create();
        mRenderPass.Create();
        mImageViews.Create();
        Renderer::Vulkan::DescriptorSetLayout layout;
        layout.Create(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        mDescriptorSetLayouts.push_back(layout);
        mCommandPool.Create();
    }

    void Root::Vulkan::Init()
    {
        mPipeline.Create(mVertShaders, mFragShaders);
        CreateFramebuffers();
        mCommandBuffer.Create();
        mUniformBuffer.Create(sizeof(UniformBufferObject));
        mDescriptorPool.Create(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        mDescriptorSet.Create();
        CreateAsyncObjects();
    }



    void Root::Vulkan::ReInit()
    {
        int width{}, height{};
        glfwGetFramebufferSize(GetWindow()->GetGLFW(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(GetWindow()->GetGLFW(), &width, &height);
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(mLogicalDevice.Get());
        DestroyFramebuffers();
        mPipeline.Destroy();
        mRenderPass.Destroy();
        mImageViews.Destroy();
        mSwapChain.Destroy();

        mSwapChain.Create();
        mImageViews.Create();
        mRenderPass.Create();
        mPipeline.Create(mVertShaders, mFragShaders);
        CreateFramebuffers();
    }

    void Root::Vulkan::Destroy()
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
        mUniformBuffer.Destroy();
        DestroyFramebuffers();
        mPipeline.Destroy();
        mImageViews.Destroy();
        mRenderPass.Destroy();
        mSwapChain.Destroy();
        mLogicalDevice.Destroy();
        mWindowSurface.Destroy();
        mInstance.Destroy();
    }

    void Root::Vulkan::AddVertexShader(VkShaderModule shader)
    {
        mVertShaders.push_back(shader);
    }

    void Root::Vulkan::AddFragmentShader(VkShaderModule shader)
    {
        mFragShaders.push_back(shader);
    }

    const Renderer::Vulkan::Instance* Root::Vulkan::GetInstance() { return &mInstance; }

    const Renderer::Vulkan::PhysicalDevice* Root::Vulkan::GetPhysDevice() { return &mPhysicalDevice; }

    const std::vector<const char*>& Root::Vulkan::GetValidationLayers() { return mValidationLayers; }

    const Renderer::Vulkan::WindowSurface* Root::Vulkan::GetWindowSurface() { return &mWindowSurface; }

    const Renderer::Vulkan::LogicalDevice* Root::Vulkan::GetLogicalDevice() { return &mLogicalDevice; }

    const Renderer::Vulkan::SwapChain* Root::Vulkan::GetSwapChain() { return &mSwapChain; }

    const Renderer::Vulkan::ImageViews* Root::Vulkan::GetImageViews() { return &mImageViews; }

    const Renderer::Vulkan::RenderPass* Root::Vulkan::GetRenderPass() { return &mRenderPass; }

    const Renderer::Vulkan::Pipeline* Root::Vulkan::GetPipeline() { return &mPipeline; }

    const std::vector<Renderer::Vulkan::Framebuffer>& Root::Vulkan::GetFramebuffers() { return mFramebuffers; }

    const Renderer::Vulkan::CommandPool* Root::Vulkan::GetCommandPool() { return &mCommandPool; }

    const Renderer::Vulkan::CommandBuffer* Root::Vulkan::GetCommandBuffer() { return &mCommandBuffer; }

    const Renderer::Vulkan::UniformBuffer* Root::Vulkan::GetUniformBuffer() { return &mUniformBuffer; }

    const Renderer::Vulkan::DescriptorPool* Root::Vulkan::GetDescriptorPool() { return &mDescriptorPool; }

    const Renderer::Vulkan::DescriptorSet* Root::Vulkan::GetDescriptorSet() { return &mDescriptorSet; }

    VkQueue* Root::Vulkan::GetGraphicsQueuePtr() { return &mGraphicsQueue; }

    VkQueue Root::Vulkan::GetGraphicsQueue() { return mGraphicsQueue; }

    VkQueue Root::Vulkan::GetPresentQueue() { return mPresentQueue; }

    VkQueue* Root::Vulkan::GetPresentQueuePtr() { return &mPresentQueue; }

    VkSemaphore Root::Vulkan::GetImageAvaibleSemaphores() { return mImageAvailableSemaphores; }

    VkSemaphore Root::Vulkan::GetRenderFinishedSemaphores() { return mRenderFinishedSemaphores; }

    VkFence Root::Vulkan::GetInFlightFence() { return mInFlightFence; }

    std::vector<VkVertexInputBindingDescription>& Root::Vulkan::GetVertexInputBindingDescriptions()
    {
        return mVertexInputBindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription>& Root::Vulkan::GetVertexInputAttributeDescriptions()
    {
        return mVertexInputAttributeDescriptions;
    }

    std::vector<Renderer::Vulkan::DescriptorSetLayout>& Root::Vulkan::GetDescriptorSetLayouts()
    {
        return mDescriptorSetLayouts;
    }

    void Root::Vulkan::UpdateCurrentImageIndex()
    {
        const VkResult result = vkAcquireNextImageKHR(mLogicalDevice.Get(), mSwapChain.Get(),
                                                      UINT64_MAX, mImageAvailableSemaphores, VK_NULL_HANDLE, &mCurrentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            ReInit();
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }
    }

    uint32_t Root::Vulkan::GetCurrentImageIndex()
    {
        return mCurrentImageIndex;
    }

    void Root::Vulkan::CreateFramebuffers()
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

    void Root::Vulkan::DestroyFramebuffers()
    {
        for (auto& framebuffer : mFramebuffers)
        {
            framebuffer.Destroy();
        }
    }

    void Root::Vulkan::CreateAsyncObjects()
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

    void Root::Vulkan::DesrtoyAsyncObjects()
    {
        const auto& device = mLogicalDevice.Get();
        vkDestroySemaphore(device, mImageAvailableSemaphores, nullptr);
        vkDestroySemaphore(device, mRenderFinishedSemaphores, nullptr);
        vkDestroyFence(device, mInFlightFence, nullptr);
    }

    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance{};
    Renderer::Vulkan::Instance Root::Vulkan::mInstance{};
    Renderer::Vulkan::PhysicalDevice Root::Vulkan::mPhysicalDevice{};
    Renderer::Vulkan::LogicalDevice Root::Vulkan::mLogicalDevice{};
    Renderer::Vulkan::WindowSurface Root::Vulkan::mWindowSurface{};
    Renderer::Vulkan::SwapChain Root::Vulkan::mSwapChain{};
    Renderer::Vulkan::ImageViews Root::Vulkan::mImageViews{};
    Renderer::Vulkan::Pipeline Root::Vulkan::mPipeline{};
    Renderer::Vulkan::RenderPass Root::Vulkan::mRenderPass{};
    std::vector<Renderer::Vulkan::Framebuffer> Root::Vulkan::mFramebuffers{};
    Renderer::Vulkan::CommandPool Root::Vulkan::mCommandPool{};
    Renderer::Vulkan::CommandBuffer Root::Vulkan::mCommandBuffer{};
    Renderer::Vulkan::UniformBuffer Root::Vulkan::mUniformBuffer{};
    Renderer::Vulkan::DescriptorPool Root::Vulkan::mDescriptorPool{};
    Renderer::Vulkan::DescriptorSet Root::Vulkan::mDescriptorSet{};
    std::vector<Renderer::Vulkan::DescriptorSetLayout> Root::Vulkan::mDescriptorSetLayouts{};
    std::vector<VkShaderModule> Root::Vulkan::mVertShaders;
    std::vector<VkShaderModule> Root::Vulkan::mFragShaders;
    std::unordered_map<std::string, Config*> Root::mConfigsMap;
    /*ResourceManager<Renderer::Texture>* Root::mTextureManager;*/
    /*std::vector<std::future<void>> Root::mFutures;*/
    /*std::string Root::mGLVersion;
    std::string Root::mGLSLVersion;*/

    /*void Root::SetGLVersion(const std::string& version) { mGLVersion = version; }

    void Root::SetGLSLVersion(const std::string& version) { mGLSLVersion = version; }*/
}
