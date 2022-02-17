//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "API/Base.hpp"
#include "API/Input.hpp"
#include "Debugger/Debugger.hpp"

#include "Renderer/OpenGL/Shader.hpp"
#include "Renderer/OpenGL/VertexArray.hpp"
#include "Renderer/OpenGL/VertexBuffer.hpp"

class BaseCommand : public oe::Input::Command
{
public:
    void execute() override
    {
        if (vec.x != 1.0f)
            vec += glm::vec3(0.1f);
    }

    void undo() override
    {
        if (vec.x > 0.0f)
            vec -= glm::vec3(0.1f);
    }

    const glm::vec3& GetVec() { return vec; }
private:
    glm::vec3 vec{};
};

class SandBoxApp : public oe::Application
{
public:
    bool Init() override
    {
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
            uniform vec3 uColor;
            void main()
            {
                FragColor = vec4(uColor, 1.0);
            }
        )";

        mShader.LoadFromSource(vSrc, fSrc);

        constexpr const float vertices[] = {
               -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };

        mVAO.Init();
        mVAO.Bind();
        mVBO.Create(sizeof(vertices), vertices);
        oe::Renderer::VertexBuffer::PushLayout(0,3,3,0);

        command = new BaseCommand;
        return true;
    }

    bool Update() override
    {
        if (oe::Input::Keyboard::isKeyPressed(oe::Input::Keyboard::Z))
            command->undo();
        if (oe::Input::Keyboard::isKeyPressed(oe::Input::Keyboard::A))
            command->execute();

        mShader.Use();
        mShader.SetUniform("uColor", command->GetVec());
        mVAO.Bind();
        gl::DrawArrays(gl::TRIANGLES, 0, 3);

        return true;
    }

    void Close() override
    {
    }

private:
    BaseCommand* command{};

    oe::Renderer::Shader mShader{};
    oe::Renderer::VertexArray mVAO{};
    oe::Renderer::VertexBuffer mVBO{};
};

std::shared_ptr<oe::Application> oe::Runtime::CreateApplication()
{
    return std::make_shared<SandBoxApp>();
}