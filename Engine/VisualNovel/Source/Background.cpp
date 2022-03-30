//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/VisualNovel/Background.hpp"

void oe::VisualNovel::Background::Init()
{
    constexpr const char* vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 uView;
                uniform mat4 uProjection;
                out vec2 TexCoords;
                void main()
                {
                    TexCoords = aPos.xy;
                    gl_Position = uView * uProjection * vec4(aPos, 1.0);
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
                    if(Texture.a < 0.1)
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

void oe::VisualNovel::Background::Load(const std::string& path)
{
    if (LoadTexture(path))
    {
        GetShader()->Use();
        GetShader()->SetUniform("uProjection", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -100.f, 100.0f));
        GetShader()->SetUniform("uView", glm::mat4(1.0f));
    }
}
