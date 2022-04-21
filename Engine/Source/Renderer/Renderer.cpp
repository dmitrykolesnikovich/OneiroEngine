//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>

#include "stb/stb_image.h"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"


void oe::Renderer::PreInit()
{
    stbi_set_flip_vertically_on_load(1);
    //GuiLayer::PreInit();
}

void oe::Renderer::Init()
{
    Core::Root::Vulkan::Init();
    //GuiLayer::Init();
}

void oe::Renderer::Shutdown()
{
    Core::Root::Vulkan::Destroy();
    //GuiLayer::Shutdown();
}

void oe::Renderer::Vulkan::BeginScene()
{
    const auto& commandBuffer = Core::Root::Vulkan::GetCommandBuffer();
    const auto& inFlightFence = Core::Root::Vulkan::GetInFlightFence();
    const auto& device = Core::Root::Vulkan::GetLogicalDevice()->Get();
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFence);

    commandBuffer->Reset();
    commandBuffer->Begin();

    Core::Root::Vulkan::GetRenderPass()->Begin();
}

void oe::Renderer::Vulkan::EndScene()
{
    Core::Root::Vulkan::GetRenderPass()->End();

    Core::Root::Vulkan::GetCommandBuffer()->End();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { Core::Root::Vulkan::GetImageAvaibleSemaphores() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = Core::Root::Vulkan::GetCommandBuffer()->GetPtr();

    VkSemaphore signalSemaphores[] = { Core::Root::Vulkan::GetRenderFinishedSemaphores() };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(Core::Root::Vulkan::GetQueue(), 1, &submitInfo, Core::Root::Vulkan::GetInFlightFence()) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { Core::Root::Vulkan::GetSwapChain()->Get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    uint32_t imageIndex = Core::Root::Vulkan::GetCurrentImageIndex();
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(Core::Root::Vulkan::GetPresentQueue(), &presentInfo);
}

//void oe::Renderer::Viewport(GLsizei width, GLsizei height)
//{
//    gl::Viewport(0, 0, width, height);
//}
//
//void oe::Renderer::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
//{
//    gl::Viewport(x, y, width, height);
//}
//
//void oe::Renderer::DrawArrays(DrawMode mode, GLint count)
//{
//    gl::DrawArrays(mode, 0, count);
//}
//
//void oe::Renderer::DrawArraysInstanced(DrawMode mode, GLsizei arraysCount, GLsizei instanceCount)
//{
//    gl::DrawArraysInstanced(mode, 0, arraysCount, instanceCount);
//}
