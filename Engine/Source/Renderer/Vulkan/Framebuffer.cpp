#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"

#include <stdexcept>

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    void Framebuffer::Create(const VkImageView* attachments)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = Core::Root::Vulkan::GetRenderPass()->Get();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = Core::Root::Vulkan::GetSwapChain()->GetExtent2D().width;
        framebufferInfo.height = Core::Root::Vulkan::GetSwapChain()->GetExtent2D().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Core::Root::Vulkan::GetLogicalDevice()->Get(),
            &framebufferInfo, nullptr, &mFramebuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }

    VkFramebuffer Framebuffer::Get() const { return mFramebuffer; }

    const VkFramebuffer* Framebuffer::GetPtr() const { return &mFramebuffer; }

    void Framebuffer::Destroy()
    {
        vkDestroyFramebuffer(Core::Root::Vulkan::GetLogicalDevice()->Get(), mFramebuffer, nullptr);
    }
}
