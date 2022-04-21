#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class LogicalDevice
    {
    public:
        void Destroy();

        [[nodiscard]] VkDevice Get() const;
        [[nodiscard]] const VkDevice* GetPtr() const;

        void Create(bool enableValidationLayers = true);
    private:
        VkDevice mDevice{};
    };
}