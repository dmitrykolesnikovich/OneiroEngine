//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Oneiro/Renderer/Gui/GuiLayer.hpp"

void oe::Renderer::PreInit()
{
    stbi_set_flip_vertically_on_load(1);
    GuiLayer::PreInit();
}

void oe::Renderer::Init()
{
    GuiLayer::Init();
}

void oe::Renderer::Shutdown()
{
    GuiLayer::Shutdown();
}

void oe::Renderer::Viewport(GLsizei width, GLsizei height)
{
    gl::Viewport(0, 0, width, height);
}

void oe::Renderer::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    gl::Viewport(x, y, width, height);
}

void oe::Renderer::DrawArrays(DrawMode mode, GLint count)
{
    gl::DrawArrays(mode, 0, count);
}

void oe::Renderer::DrawArraysInstanced(DrawMode mode, GLsizei arraysCount, GLsizei instanceCount)
{
    gl::DrawArraysInstanced(mode, 0, arraysCount, instanceCount);
}
