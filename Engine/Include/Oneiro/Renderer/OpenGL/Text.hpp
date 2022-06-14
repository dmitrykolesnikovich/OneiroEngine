//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//
#pragma once

#include "glm/glm.hpp"
#include "Base.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"

#include <map>

namespace oe::Renderer::GL
{
	class Text
	{
	private:
		struct Character
		{
			Texture<gl::TEXTURE_2D>* Texture{new Texture < gl::TEXTURE_2D >};
			glm::ivec2 Size{};
			glm::ivec2 Bearing{};
			uint32_t Advance{};
		};

	public:
		void Init();

		void SetString(const std::string& str);

		void Draw(const glm::vec2& pos = {25.0F, 25.0F},
		          const glm::vec3& color = {1.0F, 0.0F, 0.0F});
	private:
		std::map<char, Character> mCharacters{};
		std::string mString{};
		Shader mShader{};
		VertexArray mVAO{};
		Buffer<gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW> mVBO{};
	};
}
