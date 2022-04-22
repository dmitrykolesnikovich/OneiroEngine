#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class CommandPool
    {
    public:
        void Create();

        void Destroy();

        [[nodiscard]] VkCommandPool Get() const;
        [[nodiscard]] const VkCommandPool* GetPtr() const;
    private:
        VkCommandPool mCommandPool{};
    };
}
