//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/ResourceManager.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Renderer/OpenGL/Texture.hpp"
#include "Oneiro/Renderer/OpenGL/Mesh.hpp"

#include <future>
#include <deque>

namespace
{
	oe::Core::ResourceManager<oe::Renderer::GL::Texture<gl::TEXTURE_2D, true>> texturesManager;
	oe::Core::ResourceManager<oe::Renderer::GL::Mesh, const std::string> meshesManager;
}

namespace oe::Core
{
	void LoadResources()
	{
		using namespace std::chrono_literals;
		std::deque<std::future<void>> futures{};

		{ // Begin async loading meshes
			const auto& meshes = meshesManager.GetResources();
			const size_t meshesCount = meshes.size();
			for (size_t i{}; i < meshesCount; ++i)
			{
				const auto& [fst, snd] = meshes[i];
				futures.emplace_back(std::async(std::launch::async, [](
					Renderer::GL::Mesh* mesh, const std::string& path)
					{
						mesh->Load(path);
					}, fst.get(), std::get<const std::string>(snd)));
			}

			const size_t futuresCount = futures.size();
			size_t i{};
			while (i != futuresCount)
			{
				if (futures[i].wait_for(0ms) == std::future_status::ready)
				{
					meshes[i].first->Generate();
					i++;
				}
			}
		} // End async loading meshes

		futures.clear();

		{ // Begin async loading textures
			const auto& textures = texturesManager.GetResources();
			const size_t texturesCount = textures.size();
			for (size_t i{}; i < texturesCount; ++i)
			{
				const auto& [fst, snd] = textures[i];
				futures.emplace_back(std::async(std::launch::async, [](
					Renderer::GL::Texture<gl::TEXTURE_2D>* texture)
					{
						const auto& data = texture->GetData();
						if (!Renderer::GL::PreLoad2DTexture<gl::TEXTURE_2D>(*texture))
							log::get("log")->warn(
								"Failed to load texture from " + data->Path + " path");
					}, fst.get()));
			}


			const size_t futuresCount = futures.size();
			size_t i{};
			while (i != futuresCount)
			{
				if (futures[i].wait_for(0ms) == std::future_status::ready)
				{
					Load2DTexture(textures[i].first.get());
					i++;
				}
			}
		} // End async loading textures
	}

	ResourceManager<Renderer::GL::Texture<gl::TEXTURE_2D, true>>& GetTexturesManager()
	{
		return texturesManager;
	}

	ResourceManager<Renderer::GL::Mesh, const std::string>& GetMeshesManager()
	{
		return meshesManager;
	}
}
