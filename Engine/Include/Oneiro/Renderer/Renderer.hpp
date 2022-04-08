//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer
{
    enum DrawMode
    {
        TRIANGLES = gl::TRIANGLES
    };

    void PreInit();
    void Init();
    void Shutdown();
    void Viewport(GLsizei width, GLsizei height);
    void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void DrawArrays(DrawMode mode, GLint count);
    void DrawArraysInstanced(DrawMode mode, GLsizei arraysCount, GLsizei instanceCount);
}