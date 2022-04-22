//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

//#include "ResourceManager.hpp"

#include <unordered_map>
#include <vector>

#include "vulkan/vulkan_core.h"

namespace oe::Core { class Window; class Config; }
namespace oe::Runtime { class Engine; class Application; }
namespace oe::Renderer { class Texture;
    namespace Vulkan
{
    class Instance; class PhysicalDevice; class LogicalDevice; class WindowSurface;
    class SwapChain; class ImageViews; class Pipeline; class RenderPass;
    class Framebuffer; class CommandPool; class CommandBuffer; class Shader;
    class VertexBuffer; class IndexBuffer; class DescriptorSetLayout;
    class UniformBuffer; class DescriptorPool; class DescriptorSet;
} }

#include "glm/glm.hpp"

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

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
        void SetApplication(Runtime::Application* app);
        void SetWindow(Window* window);
        /*void SetGLVersion(const std::string& version);
        void SetGLSLVersion(const std::string& version);*/
    public:
        /*void LoadTexturesAsync();*/
    private:
        /*static std::vector<std::future<void>> mFutures;
        static ResourceManager<Renderer::Texture>* mTextureManager;*/
        static std::unordered_map<std::string, Config*> mConfigsMap;
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
        static void PreInit();
        static void Init();
        static void ReInit();
        static void Destroy();

        static void AddVertexShader(VkShaderModule shader);
        static void AddFragmentShader(VkShaderModule shader);

        static const Renderer::Vulkan::Instance* GetInstance();
        static const Renderer::Vulkan::PhysicalDevice* GetPhysDevice();
        static const std::vector<const char*>& GetValidationLayers();
        static const Renderer::Vulkan::WindowSurface* GetWindowSurface();
        static const Renderer::Vulkan::LogicalDevice* GetLogicalDevice();
        static const Renderer::Vulkan::SwapChain* GetSwapChain();
        static const Renderer::Vulkan::ImageViews* GetImageViews();
        static const Renderer::Vulkan::RenderPass* GetRenderPass();
        static const Renderer::Vulkan::Pipeline* GetPipeline();
        static const std::vector<Renderer::Vulkan::Framebuffer>& GetFramebuffers();
        static const Renderer::Vulkan::CommandPool* GetCommandPool();
        static const Renderer::Vulkan::CommandBuffer* GetCommandBuffer();
        static const Renderer::Vulkan::UniformBuffer* GetUniformBuffer();
        static const Renderer::Vulkan::DescriptorPool* GetDescriptorPool();
        static const Renderer::Vulkan::DescriptorSet* GetDescriptorSet();
        static VkQueue* GetGraphicsQueuePtr();
        static VkQueue GetGraphicsQueue();
        static VkQueue GetPresentQueue();
        static VkQueue* GetPresentQueuePtr();
        static VkSemaphore GetImageAvaibleSemaphores();
        static VkSemaphore GetRenderFinishedSemaphores();
        static VkFence GetInFlightFence();
        static void UpdateCurrentImageIndex();
        static uint32_t GetCurrentImageIndex();
        static std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions();

        static std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions();

        static std::vector<Renderer::Vulkan::DescriptorSetLayout>& GetDescriptorSetLayouts();
    private:
        static void CreateFramebuffers();
        static void DestroyFramebuffers();

        static void CreateAsyncObjects();
        static void DesrtoyAsyncObjects();

        static std::vector<VkShaderModule> mVertShaders;
        static std::vector<VkShaderModule> mFragShaders;
        static std::vector<Renderer::Vulkan::DescriptorSetLayout> mDescriptorSetLayouts;
        inline static std::vector<VkVertexInputBindingDescription> mVertexInputBindingDescriptions{};
        inline static std::vector<VkVertexInputAttributeDescription> mVertexInputAttributeDescriptions{};

        inline static VkQueue mGraphicsQueue{};
        inline static VkQueue mPresentQueue{};
        inline static VkSemaphore mImageAvailableSemaphores{};
        inline static VkSemaphore mRenderFinishedSemaphores{};
        inline static VkFence mInFlightFence{};

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
        static Renderer::Vulkan::UniformBuffer mUniformBuffer;
        static Renderer::Vulkan::DescriptorPool mDescriptorPool;
        static Renderer::Vulkan::DescriptorSet mDescriptorSet;
        inline static std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor" };
        inline static uint32_t mCurrentImageIndex{};
    };
}