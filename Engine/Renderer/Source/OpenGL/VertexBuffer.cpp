//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Renderer/OpenGL/VertexBuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

oe::Renderer::VertexBuffer::~VertexBuffer()
{
    gl::DeleteBuffers(1, &mID);
}

void oe::Renderer::VertexBuffer::Create(dzl::uint32_t size, const float* pVertices, dzl::int16_t type = gl::STATIC_DRAW)
{
    gl::GenBuffers(1, &mID);
    Bind();
    gl::BufferData(gl::ARRAY_BUFFER, size, pVertices, type);
}

void oe::Renderer::VertexBuffer::Bind() const
{
    gl::BindBuffer(gl::ARRAY_BUFFER, mID);
}

void oe::Renderer::VertexBuffer::UnBind() const
{
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
}

void oe::Renderer::VertexBuffer::PushLayout(dzl::uint32_t id, dzl::uint32_t size,
                                            dzl::uint32_t stride, dzl::uint32_t p)
{
    gl::EnableVertexAttribArray(id);
    gl::VertexAttribPointer(id, size, gl::FLOAT, gl::FALSE_, stride * sizeof(float), (void*)(p == 0 ? 0 : (p * sizeof(float))));
}
