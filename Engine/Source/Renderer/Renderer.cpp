//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void oe::Renderer::Init()
{
    stbi_set_flip_vertically_on_load(1);
}

void oe::Renderer::Shutdown()
{

}