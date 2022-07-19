//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <fstream>
#include <vector>

#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    VkShaderModule Shader::Create(const std::string& pathToShader)
    {
        std::ifstream file(pathToShader, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            log::get("log")->info("Failed to open shader file: " + pathToShader);

        const std::streamsize fileSize = file.tellg();

        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = buffer.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
        VkShaderModule shaderModule;

        VK_CHECK_RESULT(vkCreateShaderModule(GetLogicalDevice()->Get(), &createInfo, nullptr, &shaderModule),
                        "Failed to create shader module: " + pathToShader)

        return shaderModule;
    }

    VkVertexInputBindingDescription Shader::AddVertexInputBindingDescription(uint32_t binding, uint32_t stride)
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = binding;
        bindingDescription.stride = stride;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    VkVertexInputAttributeDescription Shader::AddVertexInputDescription(int binding, int location, VkFormat format, size_t stride,
                                                                        uint32_t offset)
    {
        VkVertexInputAttributeDescription attributeDescriptions{};
        attributeDescriptions.binding = binding;
        attributeDescriptions.location = location;
        attributeDescriptions.format = format;
        attributeDescriptions.offset = offset;
        return attributeDescriptions;
    }
} // namespace oe::Renderer::Vulkan
