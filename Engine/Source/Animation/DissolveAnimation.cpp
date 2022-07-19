//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Animation/DissolveAnimation.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"

namespace oe::Animation
{
    void DissolveAnimation::Update(Renderer::GL::Sprite2D* sprite2D, float dt)
    {
        if (mIsReversed)
        {
            if (sprite2D->GetAlpha() <= 0.0f)
            {
                sprite2D->SetAlpha(0.0f);
                mTime = 0.0f;
                return;
            }

            mTime -= dt / 1.2f;
        }
        else
        {
            if (sprite2D->GetAlpha() >= 1.0f)
            {
                sprite2D->SetAlpha(1.0f);
                mTime = 1.0f;
                return;
            }

            mTime += dt / 1.2f;
        }
        sprite2D->SetAlpha(mTime);
    }

    void DissolveAnimation::SetReversed(bool isReversed)
    {
        mIsReversed = isReversed;
    }

    bool DissolveAnimation::IsReversed()
    {
        return mIsReversed;
    }

    void DissolveAnimation::Reset()
    {
        mTime = 0.0f;
    }

    void DissolveAnimation::SetTime(float time)
    {
        mTime = time;
    }
} // namespace oe::Animation
