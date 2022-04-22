#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include <fstream>
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    void Shader::Create(const std::string& pathToShader, Type shaderType)
    {
        std::ifstream file(pathToShader, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }
        size_t fileSize = file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = buffer.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(Core::Root::Vulkan::GetLogicalDevice()->Get(),
            &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        switch (shaderType)
        {
        case VERTEX:
            Core::Root::Vulkan::AddVertexShader(shaderModule);
            break;
        case FRAGMENT: 
            Core::Root::Vulkan::AddFragmentShader(shaderModule);
            break;
        default:
            break;
        }
    }

    void Shader::AddVertexInputBindingDescription(uint32_t binding, uint32_t stride)
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = binding;
        bindingDescription.stride = stride;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        Core::Root::Vulkan::GetVertexInputBindingDescriptions().push_back(bindingDescription);
    }

    void Shader::AddVertexInputDescription(int binding, int location, VkFormat format, size_t stride, uint32_t offset)
    {
        VkVertexInputAttributeDescription attributeDescriptions;
        attributeDescriptions.binding = binding;
        attributeDescriptions.location = location;
        attributeDescriptions.format = format;
        attributeDescriptions.offset = offset;
        Core::Root::Vulkan::GetVertexInputAttributeDescriptions().push_back(attributeDescriptions);
    }
}