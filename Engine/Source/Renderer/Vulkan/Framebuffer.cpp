//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"

#include <stdexcept>

namespace oe::Renderer::Vulkan
{
    void Framebuffer::Create(const std::vector<VkImageView>& attachments)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = GetRenderPass()->Get();
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = GetSwapChain()->GetExtent2D().width;
        framebufferInfo.height = GetSwapChain()->GetExtent2D().height;
        framebufferInfo.layers = 1;

        VK_CHECK_RESULT(vkCreateFramebuffer(GetLogicalDevice()->Get(), &framebufferInfo, nullptr, &mFramebuffer),
                        "Failed to create framebuffer!")
    }

    VkFramebuffer Framebuffer::Get() const
    {
        return mFramebuffer;
    }

    const VkFramebuffer* Framebuffer::GetPtr() const
    {
        return &mFramebuffer;
    }

    void Framebuffer::Destroy()
    {
        vkDestroyFramebuffer(GetLogicalDevice()->Get(), mFramebuffer, nullptr);
    }
} // namespace oe::Renderer::Vulkan
