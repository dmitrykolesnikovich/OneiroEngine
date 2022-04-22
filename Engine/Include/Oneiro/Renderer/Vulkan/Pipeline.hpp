#pragma once

#include "Base.hpp"
#include <vector>

namespace oe::Renderer::Vulkan
{
    class Shader;
    class Pipeline
    {
    public:
        void Create(const std::vector<VkShaderModule>& vertShaderModules, 
            const std::vector<VkShaderModule>& fragShaderModules);

        void Bind() const;
        VkPipelineLayout GetLayout() const { return mPipelineLayout; }
        void Destroy();
    private:
        VkPipelineLayout mPipelineLayout{};
        VkPipeline mGraphicsPipeline{};
    };
}
