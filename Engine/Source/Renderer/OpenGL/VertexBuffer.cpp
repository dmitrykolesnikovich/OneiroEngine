//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/VertexBuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

oe::Renderer::VertexBuffer::~VertexBuffer()
{
    gl::DeleteBuffers(1, &mID);
}

void oe::Renderer::VertexBuffer::Create(uint32_t size, const float* pVertices)
{
    gl::GenBuffers(1, &mID);
    Bind();
    gl::BufferData(gl::ARRAY_BUFFER, size, pVertices, gl::STATIC_DRAW);
}

void oe::Renderer::VertexBuffer::Bind() const
{
    gl::BindBuffer(gl::ARRAY_BUFFER, mID);
}

void oe::Renderer::VertexBuffer::UnBind() const
{
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
}

void oe::Renderer::VertexBuffer::PushLayout(uint32_t id, uint32_t size, uint32_t stride, uint32_t p)
{
    gl::EnableVertexAttribArray(id);
    gl::VertexAttribPointer(id, size, gl::FLOAT, gl::FALSE_, stride * sizeof(float), (void*) (p == 0 ? 0 : (p * sizeof(float))));
}
