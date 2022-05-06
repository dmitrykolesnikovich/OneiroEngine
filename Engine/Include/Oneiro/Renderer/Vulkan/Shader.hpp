//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

#include <string>

namespace oe::Renderer::Vulkan
{
    class Shader
    {
    public:
        VkShaderModule Create(const std::string& pathToShader);
        VkVertexInputBindingDescription AddVertexInputBindingDescription(uint32_t binding, uint32_t stride);
        VkVertexInputAttributeDescription AddVertexInputDescription(int binding, int location, VkFormat format, size_t stride, uint32_t offset);
    };
}
