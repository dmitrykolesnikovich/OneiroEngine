//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>
#include <memory>
#include <string>

#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Audio
{
	class Source;
}

namespace oe::Renderer::GL
{
	class Mesh;
	class Model;

	template <int TextureType, bool Mipmaps>
	class Texture;
	class TextureData;
}

namespace oe::Core
{
	template <class T, class... Args>
	class ResourceManager
	{
	public:
		T* Add(const std::shared_ptr<T>& resourcePtr, Args... args)
		{
			mResources.push_back(std::make_pair(resourcePtr, std::make_tuple(std::move(args)...)));
			return resourcePtr.get();
		}

		void Remove(const T& resource) { mResources.erase(resource); }
		void Remove(size_t id) { mResources.erase(id); }

		[[nodiscard]] constexpr T* Get(size_t index) const { return mResources[index].first.get(); }
		[[nodiscard]] constexpr size_t Size() const { return mResources.size(); }
		constexpr std::vector<std::pair<std::shared_ptr<T>, std::tuple<Args...>>>& GetResources() { return mResources; }
	private:
		std::vector<std::pair<std::shared_ptr<T>, std::tuple<Args...>>> mResources{};
	};

	void LoadResources();

	ResourceManager<Renderer::GL::Texture<gl::TEXTURE_2D, true>>& GetTexturesManager();
	ResourceManager<Renderer::GL::Mesh, const std::string>& GetMeshesManager();
}
