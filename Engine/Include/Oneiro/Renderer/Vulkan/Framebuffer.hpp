//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    class Framebuffer
    {
    public:
        void Create(const std::vector<VkImageView>& attachments);

        [[nodiscard]] VkFramebuffer Get() const;
        [[nodiscard]] const VkFramebuffer* GetPtr() const;

        void Destroy();
    private:
        VkFramebuffer mFramebuffer{};
    };
}
