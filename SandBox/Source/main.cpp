//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Renderer/Gui/GuiLayer.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::Logger::Get("log")->PrintMessage("Initializing...");
        mBG.Init(false);
        mSprite.Init();
        mSprite.Load("sprite.png");
        mBG.Load("texture.jpg");
        return true;
    }

    bool Update() override
    {
        using namespace oe;
        using namespace Renderer;
        if (mShowGui)
        {
            auto& io = GuiLayer::GetIO();
            GuiLayer::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            GuiLayer::Begin("Hello!");
            GuiLayer::End();
        }
        mBG.Draw();
        mSprite.Draw();
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
    oe::Renderer::Sprite2D mSprite{};
    oe::Renderer::Sprite2D mBG{};
    bool mShowGui{false};
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}