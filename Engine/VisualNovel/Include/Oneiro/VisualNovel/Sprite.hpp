//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "ImageBase.hpp"

#include "Oneiro/Renderer/OpenGL/Base.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace oe::VisualNovel
{
    class OE_API Sprite : public ImageBase
    {
    public:
        void Init() override;
        void Load(const std::string& path) override;
        void Draw() override;
    };
}