//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/VertexArray.hpp"

namespace oe::Renderer::GL
{
	VertexArray::~VertexArray()
	{
		if (mID)
			gl::DeleteVertexArrays(1, &mID);
	}

	void VertexArray::Generate()
	{
		gl::GenVertexArrays(1, &mID);
	}

	void VertexArray::Bind() const
	{
		gl::BindVertexArray(mID);
	}

	void VertexArray::UnBind()
	{
		gl::BindVertexArray(0);
	}

	uint32_t VertexArray::Get() { return mID; }
}
