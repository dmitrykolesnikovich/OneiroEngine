//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Base.hpp"

namespace oe::Renderer::GL
{
	class RenderBuffer
	{
	public:
		RenderBuffer() = default;
		RenderBuffer(const RenderBuffer&) = delete;

		void Generate()
		{
			gl::GenRenderbuffers(1, &mID);
		}

		void Bind()
		{
			gl::BindRenderbuffer(gl::RENDERBUFFER, mID);
		}

		void UnBind()
		{
			gl::BindRenderbuffer(gl::RENDERBUFFER, 0);
		}

		void Storage(int internalFormat, int width, int height)
		{
			gl::RenderbufferStorage(gl::RENDERBUFFER, internalFormat, width, height);
		}

		uint32_t Get() const { return mID; }
	private:
		uint32_t mID{};
	};
}
