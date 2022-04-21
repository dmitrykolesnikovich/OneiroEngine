#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"

#include <stdexcept>

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"

namespace oe::Renderer::Vulkan
{
    void RenderPass::Create()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = Core::Root::Vulkan::GetSwapChain()->GetImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(Core::Root::Vulkan::GetLogicalDevice()->Get(), &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    void RenderPass::Begin() const
    {
        Core::Root::Vulkan::UpdateCurrentImageIndex();
        const uint32_t imageIndex = Core::Root::Vulkan::GetCurrentImageIndex();

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = Core::Root::Vulkan::GetRenderPass()->Get();
        renderPassInfo.framebuffer = Core::Root::Vulkan::GetFramebuffers()[imageIndex].Get();
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = Core::Root::Vulkan::GetSwapChain()->GetExtent2D();

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(Core::Root::Vulkan::GetCommandBuffer()->Get(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        Core::Root::Vulkan::GetPipeline()->Bind();
    }

    void RenderPass::End()
    {
        vkCmdEndRenderPass(Core::Root::Vulkan::GetCommandBuffer()->Get());
    }

    VkRenderPass RenderPass::Get() const { return mRenderPass; }

    const VkRenderPass* RenderPass::GetPtr() const { return &mRenderPass; }

    void RenderPass::Destroy()
    {
        vkDestroyRenderPass(Core::Root::Vulkan::GetLogicalDevice()->Get(), mRenderPass, nullptr);
    }
}
