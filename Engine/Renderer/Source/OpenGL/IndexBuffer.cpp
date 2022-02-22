//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Oneiro/Renderer/OpenGL/IndexBuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

oe::Renderer::IndexBuffer::~IndexBuffer()
{
    gl::DeleteBuffers(1, &mID);
}

void oe::Renderer::IndexBuffer::Init(const dzl::uint32_t* data, dzl::uint32_t count)
{
    gl::GenBuffers(1, &mID);
    Bind();
    gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, count, data, gl::STATIC_DRAW);
}

void oe::Renderer::IndexBuffer::Bind() const
{
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, mID);
}

void oe::Renderer::IndexBuffer::UnBind() const
{
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
}
