//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Animation.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"

namespace oe::Animation
{
    class DissolveAnimation final : public Animation
    {
    public:
        void Update(oe::Renderer::GL::Sprite2D* sprite2D, float dt) override
        {
            if (mIsReversed)
            {
                if (mTime <= 0.0f)
                {
                    sprite2D->SetAlpha(0.0f);
                    return;
                }
                mTime -= dt / 1.2f;
            }
            else
            {
                if (mTime >= 1.0f)
                {
                    sprite2D->SetAlpha(1.0f);
                    return;
                }
                mTime += dt / 1.2f;
            }
            sprite2D->SetAlpha(mTime);
        }

        void SetReversed(bool isReversed) { mIsReversed = isReversed; }

        bool IsReversed() { return mIsReversed; }

        void Reset() { mTime = 0.0f; }

    private:
        float mTime{};
        bool mIsReversed{};
    };
}