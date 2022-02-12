//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Renderer/Renderer.hpp"
#include "stb/stb_image.h"

void oe::Renderer::Init()
{
    stbi_set_flip_vertically_on_load(1);
}

void oe::Renderer::Shutdown()
{

}