//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

namespace oe::Renderer::GL
{
    class Sprite2D;
}

namespace oe::Animation
{
    class Animation
    {
      protected:
        ~Animation();

      public:
        virtual void Update(Renderer::GL::Sprite2D* sprite2D, float dt);
    };
} // namespace oe::Animation
