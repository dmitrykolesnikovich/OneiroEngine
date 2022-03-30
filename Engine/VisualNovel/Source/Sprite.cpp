//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/VisualNovel/Sprite.hpp"
#include "Oneiro/Core/Window.hpp"

void oe::VisualNovel::Sprite::Init()
{
    constexpr const char* vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 uView;
                uniform mat4 uProjection;
                out vec2 TexCoords;
                uniform float uAR;
                void main()
                {
                    vec2 scale = vec2(uAR > 1 ? 1 / uAR : 1, uAR < 1 ? uAR : 1);
                    TexCoords = aPos.xy;
                    gl_Position = uView * uProjection * vec4(aPos.xy * scale, 1.0, 1.0);
                }
            )";

    const char* fragmentShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                uniform sampler2D uTexture;
                in vec2 TexCoords;
                void main()
                {
                    vec4 Texture = texture2D(uTexture, TexCoords);
                    if(Texture.a < 0.35)
                            discard;
                    FragColor = vec4(Texture.rgb, 1.0f);
                }
            )";

    LoadShaderFromSource(vertexShaderSrc, fragmentShaderSrc);

    constexpr const float vertices[] = {
            1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
           -1.0f,  1.0f, 0.0f
    };

    InitBuffers(sizeof(vertices), vertices);
}

void oe::VisualNovel::Sprite::Load(const std::string& path)
{
    if (LoadTexture(path))
    {
        GetShader()->Use();
        GetShader()->SetUniform("uProjection", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f));
        GetShader()->SetUniform("uView", glm::mat4(1.0f));
    }
}

void oe::VisualNovel::Sprite::Draw()
{
    GetShader()->Use();
    GetShader()->SetUniform("uAR", Core::Window::Get()->GetAR() / GetTexture()->GetData().ar);
    DrawTexture();
}
