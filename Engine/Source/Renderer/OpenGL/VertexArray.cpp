//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/VertexArray.hpp"
#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer::GL
{
    VertexArray::~VertexArray()
    {
        gl::DeleteVertexArrays(1, &mID);
    }

    void VertexArray::Init()
    {
        gl::GenVertexArrays(1, &mID);
    }

    void VertexArray::Bind() const
    {
        gl::BindVertexArray(mID);
    }

    void VertexArray::UnBind() const
    {
        gl::BindVertexArray(0);
    }
}