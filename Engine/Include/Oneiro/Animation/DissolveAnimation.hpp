//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Animation.hpp"

namespace oe::Animation
{
    class DissolveAnimation : public Animation
    {
    public:
        void Update(oe::Renderer::GL::Sprite2D* sprite2D, float dt) override
        {
            if (mTime >= 1.0f)
            {
                sprite2D->SetAlpha(1.0f);
                return;
            }
            mTime += dt / 1.2f;
            sprite2D->SetAlpha(mTime);
        }

        void Reset() { mTime = 0.0f; }
    private:
        float mTime{};
    };
}