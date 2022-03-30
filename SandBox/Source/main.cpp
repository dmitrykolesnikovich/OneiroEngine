//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/EntryPoint.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Input.hpp"

#include "Oneiro/VisualNovel/Sprite.hpp"
#include "Oneiro/VisualNovel/Background.hpp"

class SandBoxApp : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::Logger::Get("log")->PrintMessage("Initializing...");
        bg.Init();
        sprite.Init();
        bg.Load("texture.jpg");
        sprite.Load("sprite.png");
        return true;
    }

    bool Update() override
    {
        bg.Draw();
        sprite.Draw();
        return true;
    }

    void Close() override
    {
        oe::Logger::Get("log")->PrintMessage("Closing...");
    }

    void HandleKey(oe::Input::Key key, oe::Input::Action action) override
    {
        if (action == oe::Input::PRESS)
        {
            switch (key)
            {
            case oe::Input::ESC:
                Stop();
                break;
            case oe::Input::F:
                oe::Logger::Get("log")->PrintMessage("Press F!");
                break;
            }
        }
    }

    void HandleButton(oe::Input::Button button, oe::Input::Action action) override
    {
        if (action == oe::Input::PRESS)
        {
            switch (button)
            {
            case oe::Input::LEFT:
                oe::Logger::Get("log")->PrintMessage("Press left button!");
            }
        }
    }
private:
    oe::VisualNovel::Background bg;
    oe::VisualNovel::Sprite sprite;
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}

