//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Renderer/OpenGL/Base.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Oneiro/Core/Window.hpp"
#include "ImageBase.hpp"

namespace oe::VisualNovel
{
    class Background : public ImageBase
    {
    public:
        void Init() override;
        void Load(const std::string& path) override;
    };
}