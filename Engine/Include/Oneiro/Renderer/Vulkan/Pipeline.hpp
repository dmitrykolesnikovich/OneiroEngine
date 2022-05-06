//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class Shader;

    class Pipeline
    {
    public:
        bool IsCreated() { return mGraphicsPipeline != VK_NULL_HANDLE && mPipelineLayout != VK_NULL_HANDLE; }
        void Create(VkFrontFace frontFace, bool reCreate = false);

        void Bind() const;

        [[nodiscard]] VkPipelineLayout GetLayout() const
        {
            return mPipelineLayout;
        }

        void Destroy(bool destroyShaders = false);

        void SetVertexShaderModule(VkShaderModule vertShaderModule)
        {
            if (!mVertShaderModule)
                mVertShaderModule = vertShaderModule;
        }

        void SetFragmentShaderModule(VkShaderModule fragShaderModule)
        {
            mFragShaderModule = fragShaderModule;
        }

        void SetVertexInputDescription(VkVertexInputBindingDescription binding)
        {
            mVertexInputBinding = binding;
        }

        void SetVertexInputDescription(VkVertexInputAttributeDescription attribute)
        {
            mVertexInputAttribute = attribute;
        }

    private:
        VkShaderModule mVertShaderModule{};
        VkShaderModule mFragShaderModule{};
        VkVertexInputBindingDescription mVertexInputBinding{};
        VkVertexInputAttributeDescription mVertexInputAttribute{};
        VkPipelineLayout mPipelineLayout{};
        VkPipeline mGraphicsPipeline{};
    };
}
