//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "HazelAudio/HazelAudio.h"

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Runtime/Application.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::log::get("log")->info("Initializing...");

        mSource.LoadFromFile("AcousticGuitar1.ogg");
        mSource.Play();
        mSource.SetVolume(0.01f);

        mSprite2D.Init("Textures/sprite.png");

        return true;
    }

    bool Update() override
    {
        mSprite2D.Draw();
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
            default:
                break;
            }
        }
    }


private:
    Hazel::Audio::Source mSource;
    oe::Renderer::Sprite2D mSprite2D;
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}