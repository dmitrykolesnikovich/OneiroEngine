//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <Oneiro/Lua/LuaFile.hpp>
#include "Oneiro/VisualNovel/VNCore.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Text.hpp"
#include <filesystem>

namespace
{
    oe::Renderer::GL::Text text{};
    size_t currentIt{};
    oe::Scene::SceneManager sceneManager{};
    std::vector<std::string> labels{};
    std::vector<oe::VisualNovel::Instruction> instructions{};
}

namespace oe::VisualNovel
{
    void Init(const Lua::File* file)
    {
        gl::Enable(gl::BLEND);
        gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
        text.Init();
        if (!labels.empty())
            JumpToLabel(file, labels[0]);
    }

    void NextStep()
    {
        if (currentIt >= instructions.size())
            return;
        auto instruction = instructions[currentIt];
        switch (instruction.Type)
        {
        case SHOW_BACKGROUND:
        {
            auto& sprite2D = instruction.Sprite2D;
            sprite2D->Load();
            std::filesystem::path path = sprite2D->GetTexture()->GetPath();
            sceneManager.GetScene()->GetEntity(path.filename().string())
                    .AddComponent<Sprite2DComponent>(sprite2D);
            currentIt++;
            NextStep();
            break;
        }
        case SHOW_SPRITE:
        {
            auto& sprite2D = instruction.Sprite2D;
            sprite2D->Load();
            std::filesystem::path path = sprite2D->GetTexture()->GetPath();
            sceneManager.GetScene()->GetEntity(path.filename().string())
                    .AddComponent<Sprite2DComponent>(sprite2D);
            currentIt++;
            NextStep();
            break;
        }
        case HIDE_BACKGROUND:
        {
            std::filesystem::path path = instruction.Sprite2D->GetTexture()->GetPath();
            sceneManager.GetScene()->GetEntity(path.filename().string())
                    .GetComponent<Sprite2DComponent>().Sprite2D->UnLoad();
            currentIt++;
            NextStep();
            break;
        }
        case HIDE_SPRITE:
        {
            std::filesystem::path path = instruction.Sprite2D->GetTexture()->GetPath();
            sceneManager.GetScene()->GetEntity(path.filename().string())
                    .GetComponent<Sprite2DComponent>().Sprite2D->UnLoad();
            currentIt++;
            NextStep();
            break;
        }
        case PLAY_MUSIC:
        {
            instruction.AudioSource->Play();
            currentIt++;
            NextStep();
            break;
        }
        case STOP_MUSIC:
        {
            instruction.AudioSource->Stop();
            currentIt++;
            NextStep();
            break;
        }
        case JUMP_TO_LABEL:
        {
            JumpToLabel(instruction.Label.File, instruction.Label.Name);
            NextStep();
            break;
        }
        case SAY_TEXT:
        {
            text.SetString(instruction.Text.Who + ": " + instruction.Text.What);
            currentIt++;
            return;
        }
        }
    }

    void Update()
    {
        auto view = sceneManager.GetScene()->GetEntities().view<Sprite2DComponent>();

        for (size_t i{view.storage().size()}; i > 0; --i)
        {
            auto entity = *(view.begin() + i - 1);
            view.get<Sprite2DComponent>(entity).Sprite2D->Draw();
        }
        text.Draw();
    }
    std::vector<Instruction>& GetInstructions()
    {
        return instructions;
    }

    std::vector<std::string>& GetLabels()
    {
        return labels;
    }

    void JumpToLabel(const Lua::File* file, const std::string& labelName)
    {
        for (const std::string& label : labels)
        {
            if (label == labelName)
            {
                currentIt = 0;
                instructions.clear();
                if (!file->CallFunction("main", labelName))
                    log::get("log")->warn("Failed to jump to " + labelName + " label!");
                return;
            }
        }
    }

    void ShowSprite2D(Renderer::GL::Sprite2D* sprite2D)
    {
        instructions.push_back({SHOW_SPRITE, sprite2D});
    }

    void HideSprite2D(Renderer::GL::Sprite2D* sprite2D)
    {
        instructions.push_back({HIDE_SPRITE, sprite2D});
    }

    void PlayAudioSource(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_MUSIC, {}, {}, audioSource});
    }

    void StopAudioSource(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_MUSIC, {}, {}, audioSource});
    }

    void PushInstruction(Instruction& instruction)
    {
        instructions.push_back(instruction);
    }

    void VisualNovel::PushInstruction(Instruction&& instruction)
    {
        instructions.push_back(instruction);
    }

    void PushLabel(const std::string& labelName)
    {
        labels.push_back(labelName);
    }
}