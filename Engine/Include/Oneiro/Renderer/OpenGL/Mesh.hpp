//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Oneiro/Core/ResourceManager.hpp"

struct aiMaterial;
struct aiMesh;
struct aiScene;
struct aiNode;
enum aiTextureType;

namespace oe::Renderer::GL
{
	class Mesh
	{
	public:
		Mesh() = delete;
		Mesh(const Mesh&) = delete;

		Mesh(VertexArray& vao, Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW>& vbo,
		     Buffer<gl::ELEMENT_ARRAY_BUFFER, gl::STATIC_DRAW>& ebo);

		bool Load(const std::string& path);

		bool Generate();

		const std::vector<Texture<gl::TEXTURE_2D>*>& GetTextures();

		[[nodiscard]] int GetVerticesCount() const;
		[[nodiscard]] int GetIndicesCount() const;
		[[nodiscard]] bool IsUseEBO() const;
	private:
		void ProcessAiNode(const aiNode* node, const aiScene* scene);

		void ProcessAiMesh(const aiMesh* mesh, const aiScene* scene);

		void LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type);

		std::vector<Vertex> mVertices{};
		std::vector<Texture<gl::TEXTURE_2D>*> mTextures;
		std::vector<uint32_t> mIndices{};
		
		std::string mDirectory{};

		VertexArray& mVAO;
		Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW>& mVBO;
		Buffer<gl::ELEMENT_ARRAY_BUFFER, gl::STATIC_DRAW>& mEBO;

		int mVerticesCount{};
		int mIndicesCount{};
		bool mUseEBO{};
	};
}
