//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
//#include "Oneiro/Core/Config.hpp"
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
#include "Oneiro/Core/Logger.hpp"

#include <chrono>
#include <future>

#include "Oneiro/Core/Window.hpp"

namespace oe::Core
{
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
    /*ResourceManager<Renderer::Texture>* Root::mTextureManager;*/
    /*std::vector<std::future<void>> Root::mFutures;*/
    //std::unordered_map<std::string, Config*> Root::mConfigsMap;
    /*std::string Root::mGLVersion;
    std::string Root::mGLSLVersion;*/

	Root::Root() : mVulkan(new Vulkan)
	{
		/*mTextureManager = new ResourceManager<Renderer::Texture>;*/
        /*mConfigsMap["user"] = new Config("user.cfg");
        mConfigsMap["renderer"] = new Config("renderer.cfg");*/
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

    void Root::Vulkan::Init()
    {
        mInstance.Create();
        mWindowSurface.Setup(GetWindow()->GetGLFW());
        mPhysicalDevice.Create();
        mLogicalDevice.Create();
        mSwapChain.Create();
        mRenderPass.Create();
        mImageViews.Create();
        static const auto vert = Renderer::Vulkan::Shader::CreateShaderModule(
            Renderer::Vulkan::Shader::LoadFromFile("Shaders/vert.spv"));
        static const auto frag = Renderer::Vulkan::Shader::CreateShaderModule(
            Renderer::Vulkan::Shader::LoadFromFile("Shaders/frag.spv"));
        AddShaders({ vert }, { frag });
        mPipeline.Create({ vert }, { frag });
        CreateFramebuffers();
        mCommandPool.Create();
        mCommandBuffer.Create();
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
        vkDeviceWaitIdle(mLogicalDevice.Get());
        const auto& device = mLogicalDevice.Get();
        for (const auto& fragShaderModule : mVertShaders)
            vkDestroyShaderModule(device, fragShaderModule, nullptr);
        for (const auto& vertShaderModule : mFragShaders)
            vkDestroyShaderModule(device, vertShaderModule, nullptr);
        DesrtoyAsyncObjects();
        mCommandPool.Destroy();
        DestroyFramebuffers();
        mPipeline.Destroy();
        mImageViews.Destroy();
        mRenderPass.Destroy();
        mSwapChain.Destroy();
        mLogicalDevice.Destroy();
        mWindowSurface.Destroy();
        mInstance.Destroy();
    }

    void Root::Vulkan::AddShaders(const std::vector<VkShaderModule>& vertShaders,
        const std::vector<VkShaderModule>& fragShaders)
    {
        for (const auto& vertShader : vertShaders)
            mVertShaders.push_back(vertShader);
        
        for (const auto& fragShader : fragShaders)
            mFragShaders.push_back(fragShader);
    }

    void Root::Vulkan::UpdateCurrentImageIndex()
    {
        VkResult result = vkAcquireNextImageKHR(mLogicalDevice.Get(), mSwapChain.Get(),
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
            VkImageView attachments[] = {
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

    /*void Root::SetGLVersion(const std::string& version) { mGLVersion = version; }

    void Root::SetGLSLVersion(const std::string& version) { mGLSLVersion = version; }*/
}
