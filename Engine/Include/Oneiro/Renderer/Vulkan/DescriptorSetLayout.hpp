//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    class DescriptorSetLayout
    {
    public:
        void AddBinding(int binding, VkDescriptorType type, VkShaderStageFlagBits stage);
        void Create();

        [[nodiscard]] VkDescriptorSetLayout Get() const;

        [[nodiscard]] const VkDescriptorSetLayout* GetPtr() const;

        void Destroy();
    private:
        std::vector<VkDescriptorSetLayoutBinding> mBindings;
        VkDescriptorSetLayout mDescriptorSetLayout{};
    };
}
