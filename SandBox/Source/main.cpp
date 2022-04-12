//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include "Oneiro/Renderer/OpenGL/IndexBuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::Logger::Get("log")->PrintMessage("Initializing...");
        constexpr auto vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                void main()
                {
                    gl_Position = vec4(aPos, 1.0);
                }
            )";

        constexpr auto fragmentShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
				uniform float uTime;
				uniform vec2 uSize;
                void main()
                {
					vec2 uv = gl_FragCoord.xy / uSize;
					vec4 color = vec4(0);
					color.rg += cos(uv + uTime) * cos(uTime);
					color.gb += sin(uv + uTime) * sin(uTime);
                    FragColor = color;
					FragColor.a = 1.0;
                }
            )";

        mShader.LoadFromSource(vertexShaderSrc, fragmentShaderSrc);

        constexpr float vertices[] = {
		    1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
		   -1.0f, -1.0f, 0.0f,
		   -1.0f,  1.0f, 0.0f
        };

        constexpr uint32_t indices[] = {
            0,1,3,
            1,2,3
        };

        mVAO.Init();
        mVAO.Bind();
        mEBO.Init(indices, sizeof(indices));
        mVBO.Create(sizeof(vertices), vertices);
        oe::Renderer::VertexBuffer::PushLayout(0, 3, 3, 0);
        mVAO.UnBind();
        mVBO.UnBind();
        return true;
    }

    bool Update() override
    {
        using namespace oe;
        using namespace Renderer;
        if (mShowGui)
        {
            GuiLayer::Begin("Hello!");
            GuiLayer::End();
        }
        mShader.Use();
        mShader.SetUniform("uTime", static_cast<float>(glfwGetTime()));
        mShader.SetUniform("uSize", glm::vec2(Core::Root::GetWindow()->GetData().width,
            Core::Root::GetWindow()->GetData().height));
        mVAO.Bind();
        gl::DrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_INT, nullptr);
        return true;
    }

    void Shutdown() override
    {
        oe::Logger::Get("log")->PrintMessage("Closing...");
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
            case Input::Key::F:
                if (mShowGui) SetButtonInput(true);
                else SetButtonInput(false);
                mShowGui = !mShowGui;
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
                Logger::Get("log")->PrintMessage("Press left button!");
                break;
            default:
                break;
            }
        }
    }
private:
    oe::Renderer::Shader mShader{};
    oe::Renderer::VertexBuffer mVBO;
    oe::Renderer::VertexArray mVAO;
    oe::Renderer::IndexBuffer mEBO;
    bool mShowGui{false};
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}