//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/VisualNovel/VNCore.hpp"

namespace oe::Lua
{
    class Sprite2D
    {
    public:
        Sprite2D(const std::string& path, bool keepAspectRatio)
        {
            mRendererSprite2D.Init(path, keepAspectRatio);
        }

        void Load()
        {
            mRendererSprite2D.Load();
        }

        void UnLoad()
        {
            mRendererSprite2D.UnLoad();
        }

        void Show()
        {
            VisualNovel::GetInstructions().push_back({VisualNovel::SHOW_SPRITE, this});
        }

        void Hide()
        {
            VisualNovel::GetInstructions().push_back({VisualNovel::HIDE_SPRITE, this});
        }

        Renderer::GL::Sprite2D& GetRendererSprite2D() { return mRendererSprite2D; }
    private:
        Renderer::GL::Sprite2D mRendererSprite2D;
    };
}