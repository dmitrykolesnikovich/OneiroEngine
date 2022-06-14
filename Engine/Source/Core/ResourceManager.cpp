//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/ResourceManager.hpp"
#include "Oneiro/Renderer/OpenGL/Texture.hpp"

#include <future>
#include <deque>

#include "Oneiro/Core/Logger.hpp"

namespace
{
	oe::Core::ResourceManager<oe::Renderer::GL::Texture<gl::TEXTURE_2D, true>> textureManager;
}

namespace oe::Core
{
	void LoadResources()
	{
		std::deque<std::future<void>> futures{};
		{
			const auto& textures = textureManager.GetResources();
			size_t texturesSize = textures.size();
			for (size_t i{}; i < texturesSize; ++i)
			{
				futures.emplace_back(std::async(std::launch::async, [](Renderer::GL::Texture<gl::TEXTURE_2D>* texture)
				{
					if (!PreLoad2DTexture(texture->GetData()))
						log::get("log")->warn("Failed to load texture from " + texture->GetData()->Path + " path");
				}, textures[i].get()));
			}


			size_t futuresSize = futures.size();
			size_t i{};
			while (i != futuresSize)
			{
				using namespace std::chrono_literals;
				if (futures[i].wait_for(0ms) == std::future_status::ready)
				{
					Load2DTexture(textures[i].get());
					i++;
				}
			}
		}
	}

	ResourceManager<Renderer::GL::Texture<gl::TEXTURE_2D, true>>& GetTextureManager()
	{
		return textureManager;
	}
}
