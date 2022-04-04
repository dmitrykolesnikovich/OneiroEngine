//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/EntryPoint.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"

class SandBoxApp : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::Logger::Get("log")->PrintMessage("Initializing...");
        mSprite.Init(true);
        mSprite.Load("sprite.png");
        return true;
    }

    bool Update() override
    {
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
                Logger::Get("log")->PrintMessage("Press F!");
                break;
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
            }
        }
    }
private:
    oe::Renderer::Sprite2D mSprite;
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}