//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/VertexBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/IndexBuffer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSet.hpp"
#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        namespace VkRenderer = oe::Renderer::Vulkan;

	    oe::log::get("log")->info("Initializing...");

        mVertexBuffer.Create<Vertex>(mVertices);
        mIndexBuffer.Create<uint16_t>(mIndices);
        VkRenderer::Shader::Create("Shaders/vert.spv", VkRenderer::Shader::VERTEX);
        VkRenderer::Shader::Create("Shaders/frag.spv", VkRenderer::Shader::FRAGMENT);
        VkRenderer::Shader::AddVertexInputBindingDescription(0, sizeof(Vertex));
        VkRenderer::Shader::AddVertexInputDescription(0, 0, VK_FORMAT_R32G32_SFLOAT,
                                                          sizeof(Vertex), offsetof(Vertex, Position));
        VkRenderer::Shader::AddVertexInputDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT,
                                                          sizeof(Vertex), offsetof(Vertex, Color));
        return true;
    }

    bool Update() override
    {
        using namespace oe;

        static auto startTime = std::chrono::high_resolution_clock::now();

        const auto currentTime = std::chrono::high_resolution_clock::now();
        const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        UniformBufferObject ubo{};
        ubo.model = rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f),
                                    Core::Root::Vulkan::GetSwapChain()->GetExtent2D().width /
                                    static_cast<float>(Core::Root::Vulkan::GetSwapChain()->GetExtent2D().height),
                                    0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        const auto commandBuffer = Core::Root::Vulkan::GetCommandBuffer();
        Renderer::Vulkan::BeginScene();

        mVertexBuffer.Bind(commandBuffer->Get());
        mIndexBuffer.Bind(commandBuffer->Get());
        Core::Root::Vulkan::GetUniformBuffer()->PushData<UniformBufferObject>(ubo);
        Core::Root::Vulkan::GetDescriptorSet()->Bind();
        vkCmdDrawIndexed(commandBuffer->Get(), static_cast<uint32_t>(mIndices.size()), 1, 0, 0, 0);

        Renderer::Vulkan::EndScene();

        return true;
    }

    void Shutdown() override
    {
        mVertexBuffer.Destroy();
        mIndexBuffer.Destroy();
        oe::log::get("log")->info("Closing...");
    }

    void HandleKey(oe::Input::Key key, oe::Input::Action action) override
    {
        using namespace oe;
        if (action == Input::Action::PRESS)
        {
            switch (key)
            {
            case Input::Key::ESC:
                Stop();
                break;
            default: break;
            }
        }
    }

    void HandleButton(oe::Input::Button button, oe::Input::Action action) override
    {
        using namespace oe;
        if (action == Input::Action::PRESS)
        {
            switch (button)
            {
            case Input::Button::LEFT:
                break;
            default:
                break;
            }
        }
    }

private:
    const std::vector<uint16_t> mIndices = {
        0, 1, 2, 2, 3, 0
    };

    struct Vertex
    {
        glm::vec2 Position{};
        glm::vec3 Color{};
    };

    const std::vector<Vertex> mVertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    oe::Renderer::Vulkan::VertexBuffer mVertexBuffer;
    oe::Renderer::Vulkan::IndexBuffer mIndexBuffer;
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}