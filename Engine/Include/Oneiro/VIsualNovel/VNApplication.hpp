//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <Oneiro/Lua/LuaFile.hpp>
#include "VNCore.hpp"
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Runtime/EntryPoint.hpp"

namespace oe::VisualNovel
{
    class VNApplication : public Runtime::Application
    {
    public:
        bool Init() override
        {
            mFile.LoadFile("Assets/Scripts/test.lua", false);
            VisualNovel::Init(&mFile);
            return true;
        }

        bool Update() override
        {
            VisualNovel::Update();
            return true;
        }

        void HandleButton(Input::Button button, Input::Action action) override
        {
            if (button == Input::Button::LEFT && action == Input::Action::PRESS)
                VisualNovel::NextStep();
        }

        void Shutdown() override
        {

        }
    private:
        Lua::File mFile;
    };
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<VisualNovel::VNApplication>();
    }
}