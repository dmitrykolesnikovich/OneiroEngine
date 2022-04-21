#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class CommandPool
    {
    public:
        void Create();

        void Destroy();

        VkCommandPool Get() const;
        const VkCommandPool* GetPtr() const;
    private:
        VkCommandPool mCommandPool{};
    };
}
