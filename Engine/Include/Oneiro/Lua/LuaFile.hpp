//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>
#include <Oneiro/VIsualNovel/VNCore.hpp>
#include "LuaSprite2D.hpp"
#include "LuaAudioSource.hpp"

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
            mState.set_function("showText", [](const std::string& who, const std::string& what) {
                VisualNovel::GetInstructions()
                        .push_back({VisualNovel::InstructionType::SAY_TEXT, {}, {who, what}});
            });

            mState.set_function("registerLabel", [](const std::string& name) {
                VisualNovel::GetLabels().push_back(name);
            });

            mState.set_function("jumpToLabel", [&](const std::string& name) {
                VisualNovel::GetInstructions().push_back(
                        {VisualNovel::InstructionType::JUMP_TO_LABEL, {}, {}, {}, {this, name}});
            });

            mState.new_usertype<Sprite2D>("Sprite2D", sol::call_constructor, sol::factories(
                    [](const std::string& path, bool keepAspectRatio) {
                        return std::make_shared<Sprite2D>(path, keepAspectRatio);
                    }), "show", &Sprite2D::Show, "hide", &Sprite2D::Hide);

            mState.new_usertype<AudioSource>("Audio", sol::call_constructor,
                                             sol::factories([](const std::string& path) {
                                                 return std::make_shared<AudioSource>(path);
                                             }), "play", &AudioSource::PrePlay, "stop",
                                             &AudioSource::PreStop);

            const char* characterScript = R"(
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

                function Character:init(charname)
                    self.charname = charname
                end

                function Character:__tostring()
                    return self.charname
                end

                function Character:__call(text)
                     showText(self.charname, text)
                end
            )";

            mState.script(characterScript);
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

        std::fstream& GetFile() { return mFile; }
        sol::state* GetState() { return &mState; }
    private:
        sol::state mState{};
        std::fstream mFile{};
    };
}