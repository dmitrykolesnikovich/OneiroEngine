//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include "Mesh.hpp"

namespace oe::Renderer::GL
{
	class Model
	{
	public:
		Model() = default;
		Model(const Model&) = delete;

		void Load(const std::string& path);

        void Draw() const;

        std::string GetPath();
    private:
		Mesh* mMesh{};

		std::string mPath{};
		
		VertexArray mVAO{};
		Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW> mVBO{};
		Buffer<gl::ELEMENT_ARRAY_BUFFER, gl::STATIC_DRAW> mEBO{};
	};
}
