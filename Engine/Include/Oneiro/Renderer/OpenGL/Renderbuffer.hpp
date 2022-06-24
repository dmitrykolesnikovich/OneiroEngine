//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cstdint>

namespace oe::Renderer::GL
{
	class RenderBuffer
	{
	public:
		RenderBuffer();
		RenderBuffer(const RenderBuffer&) = delete;
		~RenderBuffer();

        void Generate();

        void Bind();

        void UnBind();

        void Storage(int internalFormat, int width, int height);

        [[nodiscard]] uint32_t Get() const;
    private:
		uint32_t mID{};
	};
}
