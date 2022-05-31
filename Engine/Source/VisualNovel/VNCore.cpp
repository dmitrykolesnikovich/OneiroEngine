//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

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
    oe::Renderer::GL::Sprite2D textBox{};
    oe::Animation::DissolveAnimation textBoxAnim{};
    std::vector<std::string> labels{};
    oe::Renderer::GL::Text text{};
    std::string textToRenderer{};
    std::string textToIterate{};
    size_t currentIt{};
    uint32_t charItToPush{};
    std::vector<std::pair<oe::Renderer::GL::Sprite2D*, oe::Animation::DissolveAnimation*>>
            sprite2Ds;
    float textSpeed = 0.05f;
}

namespace oe::VisualNovel
{
    void Init(const Lua::File* file)
    {
        text.Init();
        if (!labels.empty())
        {
            JumpToLabel(file, labels[0]);
            textBox.SetAlpha(0.0f);
            textBoxAnim.SetReversed(false);
            NextStep();
        }
    }

    void NextStep()
    {
        if (currentIt >= instructions.size())
            return;
        auto& instruction = instructions[currentIt];
        static Renderer::GL::Sprite2D* prevSprite2D{};
        switch (instruction.Type)
        {
        case SHOW_SPRITE:
        {
            auto& sprite2D = instruction.Sprite2D;
            sprite2D->Load();
            sprite2D->Move({});
            sprite2D->SetAlpha(0.0f);
            sprite2Ds.push_back(std::make_pair(sprite2D, new Animation::DissolveAnimation));
            if (textBox.GetAlpha() >= 1.0f)
                textBoxAnim.SetReversed(true);
            else
                textBoxAnim.SetReversed(false);
            currentIt++;
            NextStep();
            break;
        }
        case HIDE_SPRITE:
        {
            auto it = sprite2Ds.begin();
            for (; !(it == sprite2Ds.end()); ++it)
            {
                if (it->first == instruction.Sprite2D)
                {
                    it->second->SetReversed(true);
                    textBoxAnim.SetReversed(true);
                    break;
                }
            }
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
        case PLAY_SOUND:
        {
            instruction.AudioSource->Play();
            currentIt++;
            NextStep();
            break;
        }
        case STOP_SOUND:
        {
            instruction.AudioSource->Stop();
            currentIt++;
            NextStep();
            break;
        }
        case PLAY_AMBIENT:
        {
            instruction.AudioSource->Play();
            currentIt++;
            NextStep();
            break;
        }
        case STOP_AMBIENT:
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
        static bool drawText{};
        Renderer::GL::Sprite2D* prevSprite2D{};
        for (uint32_t i = 0; i < sprite2Ds.size(); ++i)
        {
            auto* sprite2D = sprite2Ds[i].first;
            auto* animation = sprite2Ds[i].second;
            if (animation->IsReversed())
            {
                if (!textBoxAnim.IsReversed())
                {
                    static bool reverseBg{};
                    drawText =
                            sprite2Ds[0].first->GetAlpha() <= 0.0f && sprite2D->GetAlpha() <= 0.0f;
                    if (sprite2D->GetAlpha() <= 0.0f)
                    {
                        auto it = sprite2Ds.begin();
                        for (; !(it == sprite2Ds.end()); ++it)
                        {
                            if (it->first == sprite2D)
                            {
                                reverseBg = true;
                                sprite2Ds.erase(it);
                                break;
                            }
                        }
                        continue;
                    }
                    if (prevSprite2D && sprite2D->GetAlpha() >= 0.0f && !reverseBg)
                    {
                        animation->Update(sprite2D, Runtime::Engine::GetDeltaTime());
                    }
                    else if (prevSprite2D && prevSprite2D->GetAlpha() >= 0.0f && !reverseBg)
                    {
                        animation->Update(prevSprite2D, Runtime::Engine::GetDeltaTime());
                    }
                    if (sprite2Ds[0].first->GetAlpha() >= 0.0f && reverseBg)
                        sprite2Ds[0].second
                                    ->Update(sprite2Ds[0].first, Runtime::Engine::GetDeltaTime());
                }
            }
            else
            {
                if (!textBoxAnim.IsReversed() && textBox.GetAlpha() <= 0.0f)
                {
                    if (!prevSprite2D ||
                            sprite2D->GetAlpha() <= 1.0f && prevSprite2D->GetAlpha() >= 1.0f)
                        animation->Update(sprite2D, Runtime::Engine::GetDeltaTime());
                    drawText = sprite2D->GetAlpha() >= 1.0f;
                }
            }
            sprite2D->Draw();
            prevSprite2D = sprite2D;
        }

        static bool canUpdateTextLogic{};
        if (drawText)
        {
            if (textBoxAnim.IsReversed())
            {
                if (textBox.GetAlpha() >= 0.0f)
                {
                    textBoxAnim.Update(&textBox, Runtime::Engine::GetDeltaTime());
                    canUpdateTextLogic = false;
                }

                if (textBox.GetAlpha() <= 0.0f)
                {
                    textBoxAnim.SetReversed(false);
                    canUpdateTextLogic = true;
                }
            }
            else
            {
                if (textBox.GetAlpha() <= 1.0f)
                {
                    textBoxAnim.Update(&textBox, Runtime::Engine::GetDeltaTime());
                    canUpdateTextLogic = false;
                }

                if (textBox.GetAlpha() >= 1.0f)
                    canUpdateTextLogic = true;
            }

            if (canUpdateTextLogic && textBox.GetAlpha() >= 1.0f)
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
            }

            if (textBox.IsLoaded())
                textBox.Draw();
            if (textBox.GetAlpha() >= 1.0f)
                text.Draw({125.0f, 145.0f});
        }
        else
        {
            canUpdateTextLogic = false;
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
                if (!file->CallFunction("main", label))
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

    void LoadTextBox(const std::string& path)
    {
        textBox.Init(path);
        textBox.Load();
        textBox.SetUsingTextureAlpha(true);
        textBox.SetAlpha(0.0f);
    }

    void PlayMusic(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_MUSIC, {}, {}, audioSource});
    }

    void StopMusic(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_MUSIC, {}, {}, audioSource});
    }

    void PlaySound(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_SOUND, {}, {}, audioSource});
    }

    void StopSound(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_SOUND, {}, {}, audioSource});
    }

    void PlayAmbient(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_AMBIENT, {}, {}, audioSource});
    }

    void StopAmbient(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_AMBIENT, {}, {}, audioSource});
    }
}