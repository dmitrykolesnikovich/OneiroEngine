//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Texture.hpp"

namespace oe::Renderer::GL
{
	class RenderBuffer;

	class FrameBuffer
	{
	public:
		FrameBuffer() = default;
		FrameBuffer(const FrameBuffer&) = delete;
		~FrameBuffer();

		void Generate();

		void Bind();

		void UnBind();

		template <int TextureType, bool TextureMipMaps = true>
        static constexpr void Texture2D(int attachment, const Texture<TextureType, TextureMipMaps>& texture)
		{
			gl::FramebufferTexture2D(gl::FRAMEBUFFER, attachment, TextureType, texture.Get(), 0);
		}

		void RenderBuffer(int attachment, const RenderBuffer& renderBuffer);

		int Status();

		uint32_t Get();
	private:
		uint32_t mID{};
	};
}
