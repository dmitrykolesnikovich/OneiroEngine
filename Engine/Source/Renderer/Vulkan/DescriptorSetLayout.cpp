#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include <stdexcept>

namespace oe::Renderer::Vulkan
{
    void DescriptorSetLayout::Create(int binding, VkDescriptorType type, VkShaderStageFlags stageFlags)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = binding;
        uboLayoutBinding.descriptorType = type;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = stageFlags;
        uboLayoutBinding.pImmutableSamplers = nullptr; 
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;
        if (vkCreateDescriptorSetLayout(Core::Root::Vulkan::GetLogicalDevice()->Get(), &layoutInfo, 
                                        nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    VkDescriptorSetLayout DescriptorSetLayout::Get() const
    {
        return mDescriptorSetLayout;
    }

    const VkDescriptorSetLayout* DescriptorSetLayout::GetPtr() const
    {
        return &mDescriptorSetLayout;
    }

    void DescriptorSetLayout::Destroy()
    {
        vkDestroyDescriptorSetLayout(Core::Root::Vulkan::GetLogicalDevice()->Get(),
                                     mDescriptorSetLayout, nullptr);
    }
}