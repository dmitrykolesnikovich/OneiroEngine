//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"

#include <array>
#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/DepthBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"

namespace
{
    void CreateColorAttachment(VkAttachmentDescription& colorAttachment);
    void CreateDepthAttachment(VkAttachmentDescription& depthAttachment);

    void CreateColorAttachentResolve(VkAttachmentDescription& colorAttachmentResolve);
}

namespace oe::Renderer::Vulkan
{
    void RenderPass::Create()
    {
        VkAttachmentDescription colorAttachment{};
        CreateColorAttachment(colorAttachment);

        VkAttachmentDescription depthAttachment{};
        CreateDepthAttachment(depthAttachment);

        VkAttachmentDescription colorAttachmentResolve{};
        CreateColorAttachentResolve(colorAttachmentResolve);

        constexpr VkAttachmentReference
                colorAttachmentRef{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        constexpr VkAttachmentReference
                depthAttachmentRef{1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

        constexpr VkAttachmentReference
                colorAttachmentResolveRef{2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        const std::array attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;
        VK_CHECK_RESULT(vkCreateRenderPass(GetLogicalDevice()->Get(), &renderPassInfo, nullptr,
                                           &mRenderPass), "Failed to create render pass!")
    }

    void RenderPass::Begin(VkRenderPass renderPass)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = GetFramebuffers()[UpdateCurrentImageIndex()].Get();
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = GetSwapChain()->GetExtent2D();

        constexpr std::array<VkClearValue, 2>
                clearValues{{{{0.0f, 0.0f, 0.0f, 1.0f}}, {{1.0f, 0}}}};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(GetCommandBuffer()->Get(), &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPass::End()
    {
        vkCmdEndRenderPass(GetCommandBuffer()->Get());
    }

    VkRenderPass RenderPass::Get() const
    {
        return mRenderPass;
    }

    const VkRenderPass* RenderPass::GetPtr() const
    {
        return &mRenderPass;
    }

    void RenderPass::Destroy()
    {
        vkDestroyRenderPass(GetLogicalDevice()->Get(), mRenderPass, nullptr);
    }
}

namespace
{
    void CreateColorAttachment(VkAttachmentDescription& colorAttachment)
    {
        using namespace oe::Renderer::Vulkan;
        colorAttachment.format = GetSwapChain()->GetImageFormat();
        colorAttachment.samples = GetPhysDevice()->GetMsaaSamples();
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    void CreateDepthAttachment(VkAttachmentDescription& depthAttachment)
    {
        using namespace oe::Renderer::Vulkan;
        depthAttachment.format = DepthBuffer::FindDepthFormat();
        depthAttachment.samples = GetPhysDevice()->GetMsaaSamples();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    void CreateColorAttachentResolve(VkAttachmentDescription& colorAttachmentResolve)
    {
        colorAttachmentResolve.format = oe::Renderer::Vulkan::GetSwapChain()->GetImageFormat();
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
}