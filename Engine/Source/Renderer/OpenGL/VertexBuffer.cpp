//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/VertexBuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer::GL
{
    VertexBuffer::~VertexBuffer()
    {
        gl::DeleteBuffers(1, &mID);
    }

    void VertexBuffer::Create(uint32_t size, const float* pVertices, bool dynamicDraw)
    {
        gl::GenBuffers(1, &mID);
        Bind();
        gl::BufferData(gl::ARRAY_BUFFER, size, pVertices,
                       dynamicDraw ? gl::DYNAMIC_DRAW : gl::STATIC_DRAW);
    }

    void VertexBuffer::Bind() const
    {
        gl::BindBuffer(gl::ARRAY_BUFFER, mID);
    }

    void VertexBuffer::UnBind() const
    {
        gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    }

    void VertexBuffer::PushLayout(uint32_t id, int size, uint32_t stride, uint32_t p)
    {
        gl::EnableVertexAttribArray(id);
        gl::VertexAttribPointer(id, size, gl::FLOAT, gl::FALSE_, stride * sizeof(float),
                                (void*) (p == 0 ? 0 : (p * sizeof(float))));
    }

    void VertexBuffer::SubData(size_t size, const void* data)
    {
        gl::BufferSubData(gl::ARRAY_BUFFER, 0, size, data);
    }
}