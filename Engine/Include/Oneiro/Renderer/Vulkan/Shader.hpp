#pragma once

#include "Base.hpp"

#include <string>
#include <vector>

namespace oe::Renderer::Vulkan
{
    class Shader
    {
    public:
        static std::vector<char> LoadFromFile(const std::string& path);

        static VkShaderModule CreateShaderModule(const std::vector<char>& sourceCode);
    };
}
