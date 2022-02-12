//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Runtime/Application.hpp"
#include "Renderer/OpenGL/Shader.hpp"
#include "Renderer/OpenGL/VertexArray.hpp"
#include "Renderer/OpenGL/VertexBuffer.hpp"

void oe::Runtime::Application::Run()
{
    mWindow.Create(1280, 720, "Oneiro Engine");

    constexpr const char* vSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

    constexpr const char* fSrc = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    )";

    Renderer::Shader shader;
    Renderer::VertexArray VAO;
    Renderer::VertexBuffer VBO;

    shader.LoadFromSource(vSrc, fSrc);

    constexpr const float vertices[] = {
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    VAO.Init();
    VAO.Bind();
    VBO.Create(sizeof(vertices), vertices);
    Renderer::VertexBuffer::PushLayout(0,3,3,0);

    while (!mWindow.isClosed())
    {
        mWindow.PollEvents();

        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetUniform("uColor", glm::vec3(1.0f, 0.5f, 1.0f));
        VAO.Bind();
        gl::DrawArrays(gl::TRIANGLES, 0, 3);

        mWindow.Update();
    }
}
