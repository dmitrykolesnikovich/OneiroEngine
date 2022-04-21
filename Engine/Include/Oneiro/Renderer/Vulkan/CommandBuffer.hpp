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
