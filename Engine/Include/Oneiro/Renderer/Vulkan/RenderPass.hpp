//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class RenderPass
    {
    public:
        void Create();

        static void Begin(VkRenderPass renderPass);
        static void End();

        [[nodiscard]] VkRenderPass Get() const;
        [[nodiscard]] const VkRenderPass* GetPtr() const;

        void Destroy();
    private:
        VkRenderPass mRenderPass{};
    };
}
