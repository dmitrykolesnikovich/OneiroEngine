//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>

#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Scene/SceneManager.hpp"
#include "Oneiro/Scene/Entity.hpp"
#include "HazelAudio/HazelAudio.h"

namespace oe::Lua
{
    class File;
    class Sprite2D;
    class AudioSource;
}

namespace oe::VisualNovel
{
    enum InstructionType : uint8_t
    {
        SHOW_SPRITE,
        HIDE_SPRITE,
        SHOW_BACKGROUND,
        HIDE_BACKGROUND,
        PLAY_MUSIC,
        STOP_MUSIC,
        JUMP_TO_LABEL,
        SAY_TEXT
    };

    struct Instruction
    {
        InstructionType Type{};
        Lua::Sprite2D* Sprite2D{};
        struct TextData
        {
            std::string Who{};
            std::string What{};
        };
        struct LabelData
        {
            Lua::File* File;
            std::string Name{};
        };
        TextData Text;
        Lua::AudioSource* AudioSource{};
        LabelData Label;
    };

    std::vector<Instruction>& GetInstructions();
    std::vector<std::string>& GetLabels();
    void JumpToLabel(const Lua::File* file, const std::string& labelName);

    void Init(const Lua::File* file);
    void NextStep();
    void Update();
}