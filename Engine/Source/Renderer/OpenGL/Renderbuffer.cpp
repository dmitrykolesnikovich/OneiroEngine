//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Renderbuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer::GL
{
    RenderBuffer::RenderBuffer() = default;

    RenderBuffer::~RenderBuffer()
    {
        if (mID)
            gl::DeleteRenderbuffers(1, &mID);
    }

    void RenderBuffer::Generate()
    {
        gl::GenRenderbuffers(1, &mID);
    }

    void RenderBuffer::Bind()
    {
        gl::BindRenderbuffer(gl::RENDERBUFFER, mID);
    }

    void RenderBuffer::UnBind()
    {
        gl::BindRenderbuffer(gl::RENDERBUFFER, 0);
    }

    void RenderBuffer::Storage(int internalFormat, int width, int height)
    {
        gl::RenderbufferStorage(gl::RENDERBUFFER, internalFormat, width, height);
    }

    uint32_t RenderBuffer::Get() const
    {
        return mID;
    }
} // namespace oe::Renderer::GL
