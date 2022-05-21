//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <Oneiro/Lua/LuaFile.hpp>
#include "Oneiro/VisualNovel/VNCore.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Text.hpp"
#include "Oneiro/Runtime/Engine.hpp"
#include <filesystem>
#include <algorithm>
#include "Oneiro/Animation/DissolveAnimation.hpp"

namespace
{
    std::vector<oe::VisualNovel::Instruction> instructions{};
    oe::Scene::SceneManager sceneManager{};
    std::vector<std::string> labels{};
    oe::Renderer::GL::Text text{};
    std::string textToRenderer{};
    std::string textToIterate{};
    size_t currentIt{};
    uint32_t charItToPush{};
    float textSpeed = 0.05f;
    bool need2Reverse{};
}

namespace oe::VisualNovel
{
    void Init(const Lua::File* file)
    {
        text.Init();
        if (!labels.empty())
        {
            JumpToLabel(file, labels[0]);
            NextStep();
        }
    }

    void NextStep()
    {
        if (currentIt >= instructions.size())
            return;
        auto instruction = instructions[currentIt];
        switch (instruction.Type)
        {
        case SHOW_SPRITE:
        {
            auto& sprite2D = instruction.Sprite2D;
            sprite2D->Load();
            sprite2D->Move({});
            sprite2D->SetAlpha(0.0f);
            std::filesystem::path path = sprite2D->GetTexture()->GetPath();
            auto entity = sceneManager.GetScene()->GetEntity(path.filename().string());
            entity.AddComponent<Sprite2DComponent>(sprite2D);
            entity.AddComponent<AnimationComponent>(new Animation::DissolveAnimation());
            need2Reverse = true;
            currentIt++;
            NextStep();
            break;
        }
        case HIDE_SPRITE:
        {
            std::filesystem::path path = instruction.Sprite2D->GetTexture()->GetPath();
            sceneManager.GetScene()
                    ->DestroyEntity(sceneManager.GetScene()->GetEntity(path.filename().string()));
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
        case MOVE_SPRITE:
        {
            instruction.Sprite2D->Move(instruction.Vector3);
            currentIt++;
            NextStep();
            break;
        }
        case SAY_TEXT:
        {
            textToRenderer.clear();
            charItToPush = 0;
            textToIterate = instruction.Text.Who + ": " + instruction.Text.What;
            currentIt++;
            return;
        }
        case SET_TEXT_SPEED:
        {
            textSpeed = instruction.Vector3[0];
            currentIt++;
            NextStep();
        }
        }
    }

    void Update()
    {
        auto view = sceneManager.GetScene()->GetEntities()
                .view<Sprite2DComponent, AnimationComponent>();
        if (need2Reverse)
        {
            std::reverse(view.storage<Sprite2DComponent>().begin(),
                         view.storage<Sprite2DComponent>().end());
            std::reverse(view.storage<AnimationComponent>().begin(),
                         view.storage<AnimationComponent>().end());
            need2Reverse = false;
        }

        bool drawText{};
        Renderer::GL::Sprite2D* prevSprite2D{};
        for (auto entity : view)
        {
            auto* sprite2D = view.get<Sprite2DComponent>(entity).Sprite2D;
            if (!prevSprite2D || prevSprite2D->GetAlpha() >= 1.0f)
            {
                if (sprite2D->GetAlpha() <= 1.0f)
                    view.get<AnimationComponent>(entity).Animation
                            ->Update(sprite2D, Runtime::Engine::GetDeltaTime());
                sprite2D->Draw();
            }
            prevSprite2D = sprite2D;
            drawText = sprite2D->GetAlpha() >= 1.0f;
        }

        if (drawText || view.handle().empty())
        {
            static float time{};
            time += Runtime::Engine::GetDeltaTime();
            if (time >= textSpeed && charItToPush < textToIterate.size())
            {
                textToRenderer.push_back(textToIterate[charItToPush]);
                charItToPush++;
                time = 0.0f;
                text.SetString(textToRenderer);
            }
            text.Draw();
        }
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

    void MoveSprite2D(Renderer::GL::Sprite2D* sprite2D, const glm::vec3& pos)
    {
        instructions.push_back({MOVE_SPRITE, sprite2D, {}, {}, {}, pos});
    }

    void PushInstruction(Instruction&& instruction)
    {
        instructions.push_back(instruction);
    }

    void PushLabel(const std::string& labelName)
    {
        labels.push_back(labelName);
    }

    void SetTextSpeed(float speed)
    {
        instructions.push_back({SET_TEXT_SPEED, {}, {}, {}, {}, {speed, {}, {}},});
    }
}