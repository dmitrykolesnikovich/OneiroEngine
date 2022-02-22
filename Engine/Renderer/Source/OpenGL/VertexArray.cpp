//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Oneiro/Renderer/OpenGL/VertexArray.hpp"
#include "OpenGL/gl_core_4_5.hpp"

oe::VertexArray::~VertexArray()
{
    gl::DeleteVertexArrays(1, &mID);
}

void oe::VertexArray::Init()
{
    gl::GenVertexArrays(1, &mID);
}

void oe::VertexArray::Bind() const
{
    gl::BindVertexArray(mID);
}

void oe::VertexArray::UnBind() const
{
    gl::BindVertexArray(0);
}
