//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Renderer/OpenGL/Base.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Oneiro/Core/Window.hpp"

namespace oe::Renderer
{
    class OE_API Sprite2D
    {
    public:
        void Init(bool keepAspectRatio = true)
        {
            mKeepAR = keepAspectRatio;
            constexpr const char* vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 uView;
                uniform mat4 uProjection;
                out vec2 TexCoords;
                uniform float uAR;
                uniform bool uKeepAspectRatio;
                void main()
                {
                    vec2 scale = uKeepAspectRatio    ? vec2(uAR > 1 ? 1 / uAR : 1, uAR < 1 ? uAR : 1) : vec2(1.0);
                    TexCoords = aPos.xy;
                    gl_Position = uView * uProjection * vec4(aPos.xy * scale, 1.0, 1.0);
                }
            )";

            const char* fragmentShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                uniform sampler2D uTexture;
                uniform float uTextureAlpha;
                in vec2 TexCoords;
                void main()
                {
                    vec4 Texture = texture2D(uTexture, TexCoords);
                    if(Texture.a < 0.35)
                            discard;
                    FragColor = vec4(Texture.rgb, uTextureAlpha);
                }
            )";

            mShader.LoadFromSource(vertexShaderSrc, fragmentShaderSrc);

            mShader.Use();
            mShader.SetUniform("uTextureAlpha", mAlpha);
            mShader.SetUniform("uProjection", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f));
            mShader.SetUniform("uView", glm::mat4(1.0f));
            mShader.SetUniform("uKeepAspectRatio", keepAspectRatio);

            constexpr const float vertices[] = {
                    1.0f,  1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f,
                   -1.0f,  1.0f, 0.0f
            };

            mVAO.Init();
            mVAO.Bind();
            mVBO.Create(sizeof(vertices), vertices);
            Renderer::VertexBuffer::PushLayout(0,3,3,0);
            mVAO.UnBind();
            mVBO.UnBind();
        }

        bool Load(const std::string& path)
        {
            if (!mTexture.IsLoaded())
            {
                mTexture.Load(path);
                return true;
            }
            return false;
        };

        bool Load()
        {
            if (!mTexture.IsLoaded())
            {
                mTexture.Load();
                return true;
            }
            return false;
        };

        bool UnLoad()
        {
            if (mTexture.IsLoaded())
            {
                mTexture.UnLoad();
                return true;
            }
            return false;
        }

        void Draw()
        {
            if (mKeepAR)
                mShader.SetUniform("uAR", Core::Root::GetWindow()->GetData().ar / mTexture.GetData().ar);
            mVAO.Bind();
            mTexture.Bind();
            gl::DrawArrays(gl::TRIANGLES, 0, 6);
            mTexture.UnBind();
            mVAO.UnBind();
        }
    private:
        Renderer::Texture mTexture{};
        Renderer::Shader mShader{};
        Renderer::VertexBuffer mVBO;
        Renderer::VertexArray mVAO;
        float mAlpha{1.0f};
        bool mKeepAR;
    };
}