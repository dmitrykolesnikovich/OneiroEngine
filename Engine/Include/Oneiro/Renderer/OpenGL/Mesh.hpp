//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Base.hpp"
#include "Oneiro/Core/ResourceManager.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::GL
{
	struct Vertices
	{
        static std::vector<float> GetCube()
        {
            return {
			  -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			   -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			   -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

			   -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
				1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			   -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			   -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

			   -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			   -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			   -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			   -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			   -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			   -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

			   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
				1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			   -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
				1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
            };
        }
	};

	class Mesh
	{
	public:
		void Load(const std::vector<float>& vertices)
		{
			Load(vertices.data(), vertices.size() * sizeof(float));
			mVertices = vertices;
		}

		void PushTexture(const std::string& path)
		{
			const auto texture = Core::GetTextureManager().Add(std::make_shared<Texture<gl::TEXTURE_2D>>(path));
			mTextures.push_back(texture);
		}

        void Draw() const
        {
			for (size_t i{}; i < mTextures.size(); ++i)
				mTextures[i]->Bind(i);

			mVAO.Bind();
			DrawArrays(TRIANGLES, mVerticesCount);
            mVAO.UnBind();

			for (const auto& texture : mTextures)
				texture->UnBind();
		}

		[[nodiscard]] const auto& GetVertices() const
		{
			return mVertices;
		}

		[[nodiscard]] auto& GetTextures() const
		{
			return mTextures;
		}
	private:
		void Load(const float* vertices, int64_t verticesCount)
		{
			mVAO.Generate();
			mVBO.Generate();
			mVAO.Bind();
			mVBO.Bind();
			mVBO.BufferData(verticesCount, vertices);
			VertexAttribPointer<float>(0, 3, 8);
			VertexAttribPointer<float>(1, 3, 8, 3);
			VertexAttribPointer<float>(2, 2, 8, 6);
			mVAO.UnBind();
			mVBO.UnBind();
			mVerticesCount = verticesCount;
		}

		std::vector<float> mVertices{};
		std::vector<Texture<gl::TEXTURE_2D>*> mTextures{};
        int64_t mVerticesCount{};
        VertexArray mVAO{};
        Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW> mVBO{};
	};
}
