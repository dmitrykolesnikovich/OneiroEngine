//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

//#include "ResourceManager.hpp"

#include <vector>

#include "vulkan/vulkan_core.h"

namespace oe::Core { class Window; class Config; }
namespace oe::Runtime { class Engine; class Application; }
namespace oe::Renderer { class Texture;
    namespace Vulkan
{
    class Instance; class PhysicalDevice; class LogicalDevice; class WindowSurface;
    class SwapChain; class ImageViews; class Pipeline; class RenderPass;
    class Framebuffer; class CommandPool; class CommandBuffer;
} }
namespace oe::Core
{
    class Root
    {
    public:
        Root();
        ~Root();
        class Vulkan;
    public: // Getters
        static Window* GetWindow();
        static Runtime::Application* GetApplication();
        //static ResourceManager<Renderer::Texture>* GetTextureManager();
        //static std::vector<std::future<void>>& GetFutures();
        //static Config* GetConfig(const std::string& name);
        //static const std::string& GetGLVersion();
        //static const std::string& GetGLSLVersion();
        //static ALCdevice* GetALCdevice() { return mALCdevice; }
    public: // Setters (NOT STATIC!)
        /*void LoadTexturesAsync();*/
        void SetApplication(Runtime::Application* app);
        void SetWindow(Window* window);
        /*void SetGLVersion(const std::string& version);
        void SetGLSLVersion(const std::string& version);*/
    private:
        /*static std::vector<std::future<void>> mFutures;
        static ResourceManager<Renderer::Texture>* mTextureManager;*/
        //static std::unordered_map<std::string, Config*> mConfigsMap;
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
        Vulkan* mVulkan;
        /*static std::string mGLVersion;
        static std::string mGLSLVersion;*/
        //inline static ALCdevice* mALCdevice{};
    };

    class Root::Vulkan
    {
    public:
        static void Init();
        static void Destroy();

        static const Renderer::Vulkan::Instance* GetInstance() { return &mInstance; }
        static const Renderer::Vulkan::PhysicalDevice* GetPhysDevice() { return &mPhysicalDevice; }
        static const std::vector<const char*>& GetValidationLayers() { return mValidationLayers; }
        static const Renderer::Vulkan::WindowSurface* GetWindowSurface() { return &mWindowSurface; }
        static const Renderer::Vulkan::LogicalDevice* GetLogicalDevice() { return &mLogicalDevice; }
        static const Renderer::Vulkan::SwapChain* GetSwapChain() { return &mSwapChain; }
        static const Renderer::Vulkan::ImageViews* GetImageViews() { return &mImageViews; }
        static const Renderer::Vulkan::RenderPass* GetRenderPass() { return &mRenderPass; }
        static const Renderer::Vulkan::Pipeline* GetPipeline() { return &mPipeline; }
        static const std::vector<Renderer::Vulkan::Framebuffer>& GetFramebuffers() { return mFramebuffers; }
        static const Renderer::Vulkan::CommandPool* GetCommandPool() { return &mCommandPool; }
        static const Renderer::Vulkan::CommandBuffer* GetCommandBuffer() { return &mCommandBuffer; }
        static VkQueue* GetQueuePtr() { return &mGraphicsQueue; }
        static const VkQueue GetQueue() { return mGraphicsQueue; }
        static const VkQueue GetPresentQueue() { return mPresentQueue; }
        static VkQueue* GetPresentQueuePtr() { return &mPresentQueue; }
        static const VkSemaphore GetImageAvaibleSemaphores() { return mImageAvailableSemaphores; }
        static const VkSemaphore GetRenderFinishedSemaphores() { return mRenderFinishedSemaphores; }
        static const VkFence GetInFlightFence() { return mInFlightFence; }
        static void UpdateCurrentImageIndex();
        static uint32_t GetCurrentImageIndex();
    private:
        static void CreateFramebuffers();
        static void DestroyFramebuffers();

        static void CreateAsyncObjects();
        static void DesrtoyAsyncObjects();

        inline static VkQueue mGraphicsQueue{};
        inline static VkQueue mPresentQueue{};
        inline static VkSemaphore mImageAvailableSemaphores;
        inline static VkSemaphore mRenderFinishedSemaphores;
        inline static VkFence mInFlightFence;

        static Renderer::Vulkan::Instance mInstance;
        static Renderer::Vulkan::PhysicalDevice mPhysicalDevice;
        static Renderer::Vulkan::LogicalDevice mLogicalDevice;
        static Renderer::Vulkan::WindowSurface mWindowSurface;
        static Renderer::Vulkan::SwapChain mSwapChain;
        static Renderer::Vulkan::ImageViews mImageViews;
        static Renderer::Vulkan::RenderPass mRenderPass;
        static Renderer::Vulkan::Pipeline mPipeline;
        static std::vector<Renderer::Vulkan::Framebuffer> mFramebuffers;
        static Renderer::Vulkan::CommandPool mCommandPool;
        static Renderer::Vulkan::CommandBuffer mCommandBuffer;
        inline static std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };
        inline static uint32_t mCurrentImageIndex{};
    };
}