//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>

#include "Oneiro/VisualNovel/VNCore.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"

#include "sol/state.hpp"

namespace oe::Lua
{
    class File
    {
    public:
        template<class... Args>
        void OpenLibraries(Args&& ... args)
        {
            mState.open_libraries(std::forward<Args>(args)...);
        }

        sol::protected_function_result LoadFile(const std::string& pathToFile, bool out = true)
        {
            if (out)
                mFile.open(pathToFile, std::ios::out);
            OpenLibraries(sol::lib::base);
            mState.set_function("showText", [](const std::string& who, const std::string& what)
            {
            VisualNovel::PushInstruction({VisualNovel::InstructionType::SAY_TEXT, {}, {who, what}});
            });

            mState.set_function("setTextSpeed", VisualNovel::SetTextSpeed);

            mState.set_function("registerLabel", [](const std::string& name)
            {
            VisualNovel::PushLabel(name);
            });

            mState.set_function("jumpToLabel", [&](const std::string& name)
            {
            VisualNovel::PushInstruction(
                    {VisualNovel::InstructionType::JUMP_TO_LABEL, {}, {}, {}, {this, name}});
            });

            mState.new_usertype<Renderer::GL::Sprite2D>("Sprite2D", sol::call_constructor,
                                                        sol::factories([](const std::string& path,
                                                                          bool keepAspectRatio)
                                                                       {
                                                                       std::shared_ptr<Renderer::GL::Sprite2D>
                                                                               ptr =
                                                                               std::make_shared<Renderer::GL::Sprite2D>();
                                                                       ptr->Init(path,
                                                                                 keepAspectRatio);
                                                                       return ptr;
                                                                       }), "show",
                                                        &VisualNovel::ShowSprite2D, "hide",
                                                        &VisualNovel::HideSprite2D, "move",
                                                        [](Renderer::GL::Sprite2D* sprite2D,
                                                           float x, float y, float z)
                                                        {
                                                        VisualNovel::MoveSprite2D(sprite2D,
                                                                                  {x, y, z});
                                                        });

            mState.set_function("playMusic", VisualNovel::PlayMusic);
            mState.set_function("stopMusic", VisualNovel::StopMusic);
            mState.set_function("playSound", VisualNovel::PlaySound);
            mState.set_function("stopSound", VisualNovel::StopSound);
            mState.set_function("playAmbient", VisualNovel::PlayAmbient);
            mState.set_function("stopAmbient", VisualNovel::StopAmbient);

            mState.new_usertype<Hazel::Audio::Source>("Audio", sol::call_constructor,
                                                      sol::factories([](const std::string& path)
                                                                     {
                                                                     auto audioSource =
                                                                             std::make_shared<Hazel::Audio::Source>();
                                                                     audioSource
                                                                             ->LoadFromFile(path);
                                                                     return audioSource;
                                                                     }), "volume",
                                                      &Hazel::Audio::Source::SetVolume, "loop",
                                                      &Hazel::Audio::Source::SetLoop);

            const char* script = R"(
                function Class()
                local class = {}
                local mClass = {}
                class.__index = class
                function mClass:__call(...)
                    local instance = setmetatable({}, class)
                    if type(class.init) == 'function' then
                        return instance, instance:init(...)
                    end
                        return instance
                    end
                    return setmetatable(class, mClass)
                end
                Character = Class()

                function Character:init(charName)
                    self.charName = charName
                end

                function Character:__tostring()
                    return self.charName
                end

                function Character:__call(text)
                     showText(self.charName, text)
                end

                Label = Class()
                function Label:init(labelName)
                    registerLabel(labelName)
                end

                Jump = Class()
                function Jump:__call(labelName)
                    jumpToLabel(labelName)
                end
                jump = Jump()

                Music = Class()
                function Music:init(path)
                    self.src = Audio(path)
                    self.src:loop(true)
                end

                function Music:play()
                    playMusic(self.src)
                end

                function Music:stop()
                    stopMusic(self.src)
                end

                Sound = Class()
                function Sound:init(path)
                    self.src = Audio(path)
                end

                function Sound:play()
                    playSound(self.src)
                end

                function Sound:stop()
                    stopSound(self.src)
                end

                Ambient = Class()
                function Ambient:init(path)
                    self.src = Audio(path)
                    self.src:loop(true)
                end

                function Ambient:play()
                    playAmbient(self.src)
                end

                function Ambient:stop()
                    stopAmbient(self.src)
                end
            )";

            mState.script(script);
            return mState.script_file(pathToFile);
        }

        bool CallFunction(const std::string& name) const
        {
            sol::protected_function function = mState[name];
            if (function.valid())
            {
                function();
                return true;
            }
            return false;
        }

        bool CallFunction(const std::string& name, const std::string& tableName) const
        {
            sol::table table = mState[tableName];
            sol::protected_function function = table[name];
            if (function.valid())
            {
                function();
                return true;
            }
            return false;
        }

        std::fstream& GetFile() { return mFile; }
        sol::state* GetState() { return &mState; }
    private:
        sol::state mState{};
        std::fstream mFile{};
    };
}