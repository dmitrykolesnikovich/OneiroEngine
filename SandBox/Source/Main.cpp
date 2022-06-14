//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/VisualNovel/VNCore.hpp"
#include "Oneiro/Lua/LuaFile.hpp"
#include "Oneiro/Runtime/Engine.hpp"

namespace SandBox
{
    class Application : public oe::Runtime::Application
    {
    public:
        bool Init() override
        {
            Hazel::Audio::SetGlobalVolume(0.01f);
            mFile.LoadFile("Assets/Scripts/test.lua", false);
            oe::VisualNovel::Init(&mFile);
            oe::VisualNovel::LoadTextBox("Assets/Images/textbox.png");
            return true;
        }

        bool Update() override
        {
            oe::VisualNovel::Update(mRenderVnGui);
            return true;
        }

        void HandleButton(oe::Input::Button button, oe::Input::Action action) override
        {
            if (!mEnableInput)
                return;
            if (button == oe::Input::Button::LEFT && action == oe::Input::Action::PRESS)
                oe::VisualNovel::NextStep();
        }

        void HandleKey(oe::Input::Key key, oe::Input::Action action) override
        {
            if (action == oe::Input::Action::PRESS)
            {
                using namespace oe;
                if (key == Input::Key::I)
                {
                    mEnableInput = !mEnableInput;
                    return;
                }

                if (mEnableInput)
                {
                    switch (key)
                    {
                    case Input::Key::G:
                        mRenderVnGui = !mRenderVnGui;
                        break;
                    case Input::Key::SPACE:
                        VisualNovel::NextStep();
                        break;
                    case Input::Key::ENTER:
                        VisualNovel::NextStep();
                        break;
                    default: break;
                    }
                }
            }
        }

        void Shutdown() override
        {

        }

    private:
        oe::Lua::File mFile;
        bool mRenderVnGui{};
        bool mEnableInput{true};
    };
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<SandBox::Application>();
    }
}