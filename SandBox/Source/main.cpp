//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/IndexBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include "Oneiro/Renderer/Vulkan/Texture.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/VertexBuffer.hpp"
#include "Oneiro/Runtime/Application.hpp"

#include "Oneiro/Scene/Entity.hpp"
#include "Oneiro/Scene/Components.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        namespace VkRenderer = oe::Renderer::Vulkan;
        using namespace oe::Core;
        
	    oe::log::get("log")->info("Initializing...");

        mVertexBuffer.Create<Vertex>(mVertices);
        mIndexBuffer.Create<uint16_t>(mIndices);

    	VkRenderer::Shader::Create("Shaders/vert.spv", VkRenderer::Shader::VERTEX);
        VkRenderer::Shader::Create("Shaders/frag.spv", VkRenderer::Shader::FRAGMENT);

        VkRenderer::Shader::AddVertexInputBindingDescription(0, sizeof(Vertex));
        VkRenderer::Shader::AddVertexInputDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(Vertex), offsetof(Vertex, Position));
        VkRenderer::Shader::AddVertexInputDescription(0, 1, VK_FORMAT_R32G32_SFLOAT,
            sizeof(Vertex), offsetof(Vertex, TexPosition));

        mTexture.Load("Textures/texture.jpg"); // texture loading before uniform buffer creation

    	mUniformBuffer.BeginLayouts();
        mUniformBuffer.AddLayout(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        mUniformBuffer.AddLayout(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        mUniformBuffer.EndLayouts();

        mUniformBuffer.BeginBindings(sizeof(UniformBufferObject));
        mUniformBuffer.AddBinding(0, sizeof(UniformBufferObject), VK_SHADER_STAGE_VERTEX_BIT);
        mUniformBuffer.AddBinding(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, &mTexture);
        mUniformBuffer.EndBindings();

        mQuadEntity = Root::GetScene()->GetEntity("Entity");

        return true;
    }

    bool Update() override
    {
        using namespace oe;
        UniformBufferObject ubo{};

        ubo.Model = mQuadEntity.GetComponent<TransformComponent>().GetTransform();

        ubo.View = lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Proj = glm::perspective(glm::radians(45.0f), static_cast<float>(Core::Root::GetWindow()->GetWidth()) /
													     static_cast<float>(Core::Root::GetWindow()->GetHeight()),
																		    0.1f, 10.0f);
        ubo.Proj[1][1] *= -1;

        const auto commandBuffer = Renderer::Vulkan::GetCommandBuffer();
        Renderer::Vulkan::BeginScene();

        mVertexBuffer.Bind(commandBuffer->Get());
        mIndexBuffer.Bind(commandBuffer->Get());
        mUniformBuffer.PushData<UniformBufferObject>(ubo);
        vkCmdDrawIndexed(commandBuffer->Get(), static_cast<uint32_t>(mIndices.size()), 1, 0, 0, 0);

        Renderer::Vulkan::EndScene();

        return true;
    }

    void Shutdown() override
    {
        oe::log::get("log")->info("Closing...");
        mTexture.Destroy();
        mUniformBuffer.Destroy();
        mVertexBuffer.Destroy();
        mIndexBuffer.Destroy();
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

    struct Vertex
    {
        glm::vec3 Position{};
        glm::vec2 TexPosition{};
    };

    const std::vector<Vertex> mVertices = {
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}},

	    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
	    {{0.5f, -0.5f, -0.5f} , {1.0f, 0.0f}},
	    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
	    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}
    };

    const std::vector<uint16_t> mIndices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    struct UniformBufferObject
    {
        glm::mat4 Model{1.0f};
        glm::mat4 View{1.0f};
        glm::mat4 Proj{1.0f};
    };

    oe::Renderer::Vulkan::VertexBuffer mVertexBuffer;
    oe::Renderer::Vulkan::IndexBuffer mIndexBuffer;
    oe::Renderer::Vulkan::UniformBuffer mUniformBuffer;
    oe::Renderer::Vulkan::Texture mTexture;
    
    oe::Scene::Entity mQuadEntity{};
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}