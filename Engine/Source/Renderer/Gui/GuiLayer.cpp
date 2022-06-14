#include "Oneiro/Renderer/Gui/GuiLayer.hpp"

#include "Oneiro/Core/Window.hpp"

#ifndef OE_RENDERER_VULKAN

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Oneiro/Core/Root.hpp"

namespace oe::Renderer::GuiLayer
{
    void PreInit()
    {
        IMGUI_CHECKVERSION();
        CreateContext();
        StyleColorsDark();
    }

    void Init()
    {
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(Core::Root::GetWindow()->GetGLFW(), true);
    }

    void NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Draw()
    {
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
    }

    void Shutdown()
    {
        ImGui_ImplGlfw_Shutdown();
        DestroyContext();
    }

}

#else
#include "Oneiro/Renderer/Vulkan/DescriptorPool.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"

namespace oe::Renderer::GuiLayer
{
    void PreInit()
    {
        IMGUI_CHECKVERSION();
        CreateContext();
        StyleColorsDark();
    }

    void Init()
    {
        ImGui_ImplGlfw_InitForVulkan(Core::Root::GetWindow()->GetGLFW(), true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = Vulkan::GetInstance()->Get();
        init_info.PhysicalDevice = Vulkan::GetPhysDevice()->Get();
        init_info.Device = Vulkan::GetLogicalDevice()->Get();
        init_info.QueueFamily = static_cast<uint32_t>(-1);
        init_info.Queue = Vulkan::GetPresentQueue();
        init_info.DescriptorPool = Vulkan::GetGuiDescriptorPool();
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_2_BIT;
        ImGui_ImplVulkan_Init(&init_info, Vulkan::GetRenderPass()->Get());

        VK_CHECK_RESULT(vkResetCommandPool(Vulkan::GetLogicalDevice()->Get(), Vulkan::GetCommandPool()->Get(), 0), "Failed to reset command pool!")
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VK_CHECK_RESULT(vkBeginCommandBuffer(Vulkan::GetCommandBuffer()->Get(), &begin_info), "Failed to begin command buffer!")
        ImGui_ImplVulkan_CreateFontsTexture(Vulkan::GetCommandBuffer()->Get());
        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = Vulkan::GetCommandBuffer()->GetPtr();
        VK_CHECK_RESULT(vkEndCommandBuffer(Vulkan::GetCommandBuffer()->Get()), "Failed to record buffer!")
        VK_CHECK_RESULT(vkQueueSubmit(Vulkan::GetGraphicsQueue(), 1, &end_info, VK_NULL_HANDLE), "Failed to submit queue!")

        Vulkan::GetLogicalDevice()->WaitIdle();
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void NewFrame()
    {
        //Vulkan::BeginGuiScene();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Draw()
    {
        Render();
        ImGui_ImplVulkan_RenderDrawData(GetDrawData(), Vulkan::GetCommandBuffer()->Get());

        //Vulkan::EndGuiScene();
    }

    void Shutdown()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        DestroyContext();
    }

}

#endif