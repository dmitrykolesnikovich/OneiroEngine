//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"

#include <array>

#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include <vector>
#include <stdexcept>

namespace
{
    void CreateDepthStencilStateInfo(VkPipelineDepthStencilStateCreateInfo& depthStencil);
    void CreateVertexInputStateInfo(const VkVertexInputBindingDescription* binding,
                                    const VkVertexInputAttributeDescription* attribute,
                                    VkPipelineVertexInputStateCreateInfo& vertexInputInfo);
    void CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& inputAssembly);
    void CreateViewport(VkViewport& viewport);
    void CreateScissor(VkRect2D& scissor);
    void CreateViewportStateInfo(VkViewport& viewport, VkRect2D& scissor,
                                 VkPipelineViewportStateCreateInfo& viewportState);
    void CreateRasterizationStateInfo(VkFrontFace frontFace,
                                      VkPipelineRasterizationStateCreateInfo& rasterizer);
    void CreateMultiSamplingStateInfo(VkPipelineMultisampleStateCreateInfo& multisampling);
    void CreateColorBlendAttachmentState(VkPipelineColorBlendAttachmentState& colorBlendAttachment);
    void CreateColorBlendStateInfo(const VkPipelineColorBlendAttachmentState& colorBlendAttachment,
                                   VkPipelineColorBlendStateCreateInfo& colorBlending);
    void CreateDynamicStateInfo(VkPipelineDynamicStateCreateInfo& dynamicState);
    void CreatePipelineLayoutInfo(bool is3d, VkPipelineLayoutCreateInfo& pipelineLayoutInfo);
    VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits stage,
                                                          VkShaderModule shaderModule);
}

namespace oe::Renderer::Vulkan
{
    void Pipeline::Create(VkFrontFace frontFace, bool reCreate)
    {
        if (!reCreate && mGraphicsPipeline)
            return;
        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
        const auto& device = GetLogicalDevice()->Get();

        shaderStages[0] = CreateShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, mVertShaderModule);
        shaderStages[1] = CreateShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, mFragShaderModule);

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        CreateVertexInputStateInfo(&mVertexInputBinding, &mVertexInputAttribute, vertexInputInfo);
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        CreateInputAssemblyStateInfo(inputAssembly);
        VkViewport viewport{};
        CreateViewport(viewport);

        VkRect2D scissor{};
        CreateScissor(scissor);

        VkPipelineViewportStateCreateInfo viewportState{};
        CreateViewportStateInfo(viewport, scissor, viewportState);

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        CreateRasterizationStateInfo(frontFace, rasterizer);

        VkPipelineMultisampleStateCreateInfo multisampling{};
        CreateMultiSamplingStateInfo(multisampling);

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        CreateColorBlendAttachmentState(colorBlendAttachment);

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        CreateColorBlendStateInfo(colorBlendAttachment, colorBlending);

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        CreateDepthStencilStateInfo(depthStencil);

        VkPipelineDynamicStateCreateInfo dynamicState{};
        CreateDynamicStateInfo(dynamicState);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        CreatePipelineLayoutInfo(frontFace == VK_FRONT_FACE_COUNTER_CLOCKWISE, pipelineLayoutInfo);

        VK_CHECK_RESULT(
                vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &mPipelineLayout),
                "Failed to create pipeline layput!")

        VkGraphicsPipelineCreateInfo pipelineInfo{};

        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = mPipelineLayout;
        pipelineInfo.renderPass = GetRenderPass()->Get();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        if (!mGraphicsPipeline)
            AddPipeline(this);

        // TODO: Add pipeline cache
        VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
                                                  &mGraphicsPipeline),
                        "Failed to create graphics pipeline!")
    }

    void Pipeline::Bind() const
    {
        Renderer::Vulkan::SetCurrentPipeline(const_cast<Pipeline*>(this));
        vkCmdBindPipeline(GetCommandBuffer()->Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                          mGraphicsPipeline);
    }

    void Pipeline::Destroy(bool destroyShaders)
    {
        const auto device = GetLogicalDevice()->Get();

        if (destroyShaders)
        {
            if (mVertShaderModule)
                vkDestroyShaderModule(device, mVertShaderModule, nullptr);
            if (mFragShaderModule)
                vkDestroyShaderModule(device, mFragShaderModule, nullptr);
        }

        vkDestroyPipeline(device, mGraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);
    }
}

namespace
{
    void CreateDepthStencilStateInfo(VkPipelineDepthStencilStateCreateInfo& depthStencil)
    {
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};
    }

    void CreateVertexInputStateInfo(const VkVertexInputBindingDescription* binding,
                                    const VkVertexInputAttributeDescription* attribute,
                                    VkPipelineVertexInputStateCreateInfo& vertexInputInfo)
    {
        using namespace oe::Renderer::Vulkan;

        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = binding;
        vertexInputInfo.vertexAttributeDescriptionCount = 1;
        vertexInputInfo.pVertexAttributeDescriptions = attribute;
    }

    void CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& inputAssembly)
    {
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
    }

    void CreateViewport(VkViewport& viewport)
    {
        using namespace oe::Renderer::Vulkan;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(GetSwapChain()->GetExtent2D().width);
        viewport.height = static_cast<float>(GetSwapChain()->GetExtent2D().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
    }

    void CreateScissor(VkRect2D& scissor)
    {
        scissor.offset = {0, 0};
        scissor.extent = oe::Renderer::Vulkan::GetSwapChain()->GetExtent2D();
    }

    void CreateViewportStateInfo(VkViewport& viewport, VkRect2D& scissor,
                                 VkPipelineViewportStateCreateInfo& viewportState)
    {
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
    }

    void CreateRasterizationStateInfo(VkFrontFace frontFace,
                                      VkPipelineRasterizationStateCreateInfo& rasterizer)
    {
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_TRUE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = frontFace;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;
    }

    void CreateMultiSamplingStateInfo(VkPipelineMultisampleStateCreateInfo& multisampling)
    {
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_TRUE;
        multisampling.rasterizationSamples =
                oe::Renderer::Vulkan::GetPhysDevice()->GetMsaaSamples();
        multisampling.minSampleShading = .2f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;
    }

    void CreateColorBlendAttachmentState(VkPipelineColorBlendAttachmentState& colorBlendAttachment)
    {
        colorBlendAttachment.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    void CreateColorBlendStateInfo(const VkPipelineColorBlendAttachmentState& colorBlendAttachment,
                                   VkPipelineColorBlendStateCreateInfo& colorBlending)
    {
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
    }

    void CreateDynamicStateInfo(VkPipelineDynamicStateCreateInfo& dynamicState)
    {
        const std::vector dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();
    }

    void CreatePipelineLayoutInfo(bool is3d, VkPipelineLayoutCreateInfo& pipelineLayoutInfo)
    {
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        if (is3d)
        {
            const std::vector<VkDescriptorSetLayout>
                    & descriptorSetLayouts = oe::Renderer::Vulkan::GetDescriptorSetLayouts3D();
            pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
            pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        }
        else
        {
            const std::vector<VkDescriptorSetLayout>
                    & descriptorSetLayouts = oe::Renderer::Vulkan::GetDescriptorSetLayouts();
            pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
            pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        }
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
    }

    VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits stage,
                                                          VkShaderModule shaderModule)
    {
        VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};

        shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo.stage = stage;
        shaderStageCreateInfo.module = shaderModule;
        shaderStageCreateInfo.pName = "main";

        return shaderStageCreateInfo;
    }
}
