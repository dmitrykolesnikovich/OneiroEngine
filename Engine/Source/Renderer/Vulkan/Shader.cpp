#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include <fstream>
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    std::vector<char> Shader::LoadFromFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    VkShaderModule Shader::CreateShaderModule(const std::vector<char>& sourceCode)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = sourceCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(Core::Root::Vulkan::GetLogicalDevice()->Get(),
            &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }
}