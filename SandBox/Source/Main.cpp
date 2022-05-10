//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Core/ResourceManager.hpp"
#include "HazelAudio/HazelAudio.h"
#include "Oneiro/Renderer/OpenGL/ErrorHandler.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        using namespace oe;
        oe::log::get("log")->info("Initializing...");
        mSprite2D.Init("Assets/Textures/sprite.png");
        mSource.LoadFromFile("Assets/Audio/music.mp3");
        mSource.SetVolume(0.45f);
        mSource.Play();
        return true;
    }

    bool Update() override
    {
        oe::Renderer::GL::ErrorHandler::GLClearError();
        mSprite2D.Draw();
        oe::Renderer::GL::ErrorHandler::GLLogCall();
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
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<SandBoxApp>();
    }
}