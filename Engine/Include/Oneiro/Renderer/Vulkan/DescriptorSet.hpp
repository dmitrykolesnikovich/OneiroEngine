//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Base.hpp"

namespace oe::Renderer::Vulkan
{
    class DescriptorSet
    {
    public:
        void Create(VkBuffer buffer, int binding, VkDeviceSize size, const VkDescriptorSetLayout* layout, VkDescriptorPool pool);
        void Bind() const;
    private:
        VkDescriptorSet mDescriptorSet{};
    };
}
