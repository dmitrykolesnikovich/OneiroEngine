//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/EntryPoint.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "imgui.h"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Runtime/Application.hpp"

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
        mBG.Draw();
        mSprite.Draw();
        return true;
    }

    bool UpdateGui() override
    {
        ImGui::Begin("Test");
        ImGui::End();
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
                SetButtonInput(false);
                break;
            case Input::Key::G:
                SetButtonInput(true);
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
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}