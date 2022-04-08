//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"
#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer
{
    enum DrawMode
    {
        TRIANGLES = gl::TRIANGLES
    };

    OE_API void PreInit();
    OE_API void Init();
    OE_API void Shutdown();
    OE_API void Viewport(GLsizei width, GLsizei height);
    OE_API void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    OE_API void DrawArrays(DrawMode mode, GLint count);
    OE_API void DrawArraysInstanced(DrawMode mode, GLsizei arraysCount, GLsizei instanceCount);
}