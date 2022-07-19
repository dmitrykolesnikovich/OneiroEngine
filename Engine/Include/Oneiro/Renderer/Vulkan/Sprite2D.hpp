//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "IndexBuffer.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace oe::Renderer::Vulkan
{
    class Sprite2D
    {
      public:
        void Load(const std::string& pathFile, bool keepAR = true)
        {
            mVertexBuffer.Create<Vertex>(mVertices);
            mIndexBuffer.Create<uint16_t>(mIndices);

            mTexture.Load(pathFile); // texture loading before uniform buffer creation

            mUniformBuffer.BeginLayouts();
            mUniformBuffer.AddLayout(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
            mUniformBuffer.AddLayout(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
            mUniformBuffer.EndLayouts(false);

            mUniformBuffer.BeginBindings(sizeof(Uniforms), 2);
            mUniformBuffer.AddBinding(0, sizeof(Uniforms), VK_SHADER_STAGE_VERTEX_BIT);
            mUniformBuffer.AddBinding(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, &mTexture);
            mUniformBuffer.EndBindings();

            mUniforms.KeepAR = keepAR;
            mKeepAR = keepAR;
            mUniforms.Projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);

            if (!mPipeline.IsCreated())
            {
                mPipeline.SetVertexShaderModule(mVertShader.Create("Shaders/2D/Compiled/sprite2d.vert.spv"));
                mPipeline.SetFragmentShaderModule(mFragShader.Create("Shaders/2D/Compiled/sprite2d.frag.spv"));
                mPipeline.SetVertexInputDescription(mVertShader.AddVertexInputBindingDescription(0, sizeof(glm::vec2)));
                mPipeline.SetVertexInputDescription(
                    mVertShader.AddVertexInputDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, sizeof(glm::vec2), 0));

                mPipeline.Create(VK_FRONT_FACE_CLOCKWISE);
            }
        }

        void Load()
        {
            mTexture.Load();
        }

        void UnLoad()
        {
            mTexture.UnLoad();
        }

        void Destroy()
        {
            mTexture.Destroy();
            mIndexBuffer.Destroy();
            mVertexBuffer.Destroy();
            mUniformBuffer.Destroy();
        }

        void Draw()
        {
            mPipeline.Bind();
            if (mKeepAR)
                mUniforms.AR = Core::Root::GetWindow()->GetData().ar / mTexture.GetData().Ar;
            mUniforms.Model = translate(glm::mat4(1.0f), Position);
            mVertexBuffer.Bind();
            mIndexBuffer.Bind();
            mUniformBuffer.PushData<Uniforms>(mUniforms);
            DrawIndexed(static_cast<uint32_t>(mIndices.size()));
        }

        glm::vec3 Position{0.0f};

      private:
        struct Uniforms
        {
            glm::mat4 View{1.0f};
            glm::mat4 Projection{1.0f};
            glm::mat4 Model{1.0f};
            float AR{};
            bool KeepAR{};
        } mUniforms;

        const std::vector<uint16_t> mIndices = {0, 1, 2, 2, 3, 0};

        struct Vertex
        {
            glm::vec2 Position{};
        };

        const std::vector<Vertex> mVertices = {{{-1.0f, -1.0f}}, {{1.0f, -1.0f}}, {{1.0f, 1.0f}}, {{-1.0f, 1.0f}}};
        Shader mVertShader{};
        Shader mFragShader{};
        Texture mTexture{};
        VertexBuffer mVertexBuffer{};
        IndexBuffer mIndexBuffer{};
        UniformBuffer mUniformBuffer{};

        inline static Pipeline mPipeline;

        bool mKeepAR{};
    };
} // namespace oe::Renderer::Vulkan
