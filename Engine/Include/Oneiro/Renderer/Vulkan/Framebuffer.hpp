#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class Framebuffer
    {
    public:
        void Create(const VkImageView* attachments);

        [[nodiscard]] VkFramebuffer Get() const;
        [[nodiscard]] const VkFramebuffer* GetPtr() const;

        void Destroy();
    private:
        VkFramebuffer mFramebuffer{};
    };
}
