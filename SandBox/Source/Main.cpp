//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/VisualNovel/VNCore.hpp"
#include "Oneiro/Lua/LuaFile.hpp"

namespace SandBox
{
    class Application : public oe::Runtime::Application
    {
    public:
        bool Init() override
        {
            mFile.LoadFile("Assets/Scripts/test.lua", false);
            oe::VisualNovel::Init(&mFile);
            return true;
        }

        bool Update() override
        {
            oe::VisualNovel::Update();
            return true;
        }

        void HandleButton(oe::Input::Button button, oe::Input::Action action) override
        {
            if (button == oe::Input::Button::LEFT && action == oe::Input::Action::PRESS)
                oe::VisualNovel::NextStep();
        }

        void Shutdown() override
        {

        }
    private:
        oe::Lua::File mFile;
    };
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<SandBox::Application>();
    }
}