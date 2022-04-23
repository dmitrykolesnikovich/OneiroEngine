//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

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
        [[nodiscard]] VkPipelineLayout GetLayout() const { return mPipelineLayout; }
        void Destroy();
    private:
        VkPipelineLayout mPipelineLayout{};
        VkPipeline mGraphicsPipeline{};
    };
}
