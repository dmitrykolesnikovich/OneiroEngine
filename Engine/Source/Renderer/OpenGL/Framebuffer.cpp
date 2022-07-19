//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Framebuffer.hpp"
#include "Oneiro/Renderer/OpenGL/Renderbuffer.hpp"

namespace oe::Renderer::GL
{
    FrameBuffer::~FrameBuffer()
    {
        if (mID)
            gl::DeleteFramebuffers(1, &mID);
    }

    void FrameBuffer::Generate()
    {
        gl::GenFramebuffers(1, &mID);
    }

    void FrameBuffer::Bind()
    {
        gl::BindFramebuffer(gl::FRAMEBUFFER, mID);
    }

    void FrameBuffer::UnBind()
    {
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
    }

    void FrameBuffer::RenderBuffer(int attachment, const GL::RenderBuffer& renderBuffer)
    {
        gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, attachment, gl::RENDERBUFFER, renderBuffer.Get());
    }

    int FrameBuffer::Status()
    {
        return gl::CheckFramebufferStatus(gl::FRAMEBUFFER);
    }

    uint32_t FrameBuffer::Get()
    {
        return mID;
    }
} // namespace oe::Renderer::GL
