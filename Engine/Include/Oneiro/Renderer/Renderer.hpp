//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

namespace oe::Renderer
{
    void PreInit();
    void Init();
    void Shutdown();
}

#ifndef OE_VULKAN_API

#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer::GL
{
    enum DrawMode
    {
        TRIANGLES = gl::TRIANGLES
    };

    void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void Viewport(GLsizei width, GLsizei height);
    void DrawArrays(DrawMode mode, GLint count);
    [[maybe_unused]] void DrawArraysInstanced(DrawMode mode, GLsizei arraysCount,
                                              GLsizei instanceCount);
}
#else
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "vulkan/vulkan_core.h"

#include "Oneiro/Core/Oneiro.hpp"

#include <vector>

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
    class DescriptorSetLayout;
    class DescriptorSet;

    void ReCreateSwapChain();

    void BeginScene();
    void BeginGuiScene();
    void EndGuiScene();
    void EndScene();

    void Bind2DPipeline();
    void Bind3DPipeline();

    void Draw(uint32_t verticesCount);
    void DrawIndexed(uint32_t indexCount);

    void AddVertexShader(VkShaderModule shader);
    void AddFragmentShader(VkShaderModule shader);

    uint32_t UpdateCurrentImageIndex();
    VkDescriptorPool GetGuiDescriptorPool();

    const Instance* GetInstance();
    const PhysicalDevice* GetPhysDevice();
    const std::vector<const char*>& GetValidationLayers();
    const WindowSurface* GetWindowSurface();
    const SwapChain* GetGuiSwapChain();
    const LogicalDevice* GetLogicalDevice();
    const SwapChain* GetSwapChain();
    const ImageViews* GetImageViews();
    const RenderPass* GetRenderPass();
    const std::vector<Framebuffer>& GetFramebuffers();
    const CommandPool* GetCommandPool();
    const CommandBuffer* GetCommandBuffer();
    const DescriptorSet* GetDescriptorSet();
    VkQueue* GetGraphicsQueuePtr();
    VkQueue GetGraphicsQueue();
    VkQueue GetPresentQueue();
    VkQueue* GetPresentQueuePtr();
    VkSemaphore GetImageAvailableSemaphores();
    VkSemaphore GetRenderFinishedSemaphores();
    VkFence GetInFlightFence();
    uint32_t GetCurrentImageIndex();
    Pipeline* GetCurrentPipeline();

    void SetCurrentPipeline(Pipeline* pipeline);
    void AddPipeline(Pipeline* pipeline);

    std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions();
    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts();
    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts3D();
}
#endif