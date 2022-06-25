//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Animation.hpp"

namespace oe::Animation
{
	class DissolveAnimation final : public Animation
	{
	public:
		void Update(Renderer::GL::Sprite2D* sprite2D, float dt) override;
        void SetReversed(bool isReversed);
        bool IsReversed();
        void Reset();
        void SetTime(float time);
    private:
		float mTime{};
		bool mIsReversed{};
	};
}
