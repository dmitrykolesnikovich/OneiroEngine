//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class CommandBuffer
    {
    public:
        void Create();

        void Begin() const;

        void End() const;

        void Reset() const;

        [[nodiscard]] VkCommandBuffer Get() const;
        [[nodiscard]] const VkCommandBuffer* GetPtr() const;
    private:
        VkCommandBuffer mCommandBuffer{};
    };
}
