#pragma once

#include "Base.hpp"

#include <string>

#include "Oneiro/Core/Root.hpp"

namespace oe::Renderer::Vulkan
{
    class Shader
    {
    public:
        enum Type
        {
            VERTEX,
            FRAGMENT
        };
        static void Create(const std::string& pathToShader, Type shaderType);
        static void AddVertexInputBindingDescription(uint32_t binding, uint32_t stride);
        static void AddVertexInputDescription(int binding, int location, VkFormat format, size_t stride, uint32_t offset);
    };
}
