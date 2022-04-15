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
	    oe::log::get("log")->info("Initializing...");
        mShader.LoadFromFile("Shaders/shader.glsl");

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