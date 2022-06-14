//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>
#include <memory>

#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Audio
{
	class Source;
}

namespace oe::Renderer::GL
{
	template <int TextureType, bool Mipmaps>
	class Texture;

	class TextureData;
}

namespace oe::Core
{
	template <class T>
	class ResourceManager
	{
	public:
		T* Add(const std::shared_ptr<T>& resourcePtr)
		{
			mResources.emplace_back(resourcePtr);
			return resourcePtr.get();
		}

		void Remove(const T& resource) { mResources.erase(resource); }
		void Remove(size_t id) { mResources.erase(id); }

		[[nodiscard]] constexpr T* Get(size_t index) const { return mResources[index].get(); }
		[[nodiscard]] constexpr size_t Size() const { return mResources.size(); }
		constexpr std::vector<std::shared_ptr<T>>& GetResources() { return mResources; }
	private:
		std::vector<std::shared_ptr<T>> mResources{};
	};

	void LoadResources();

	ResourceManager<Renderer::GL::Texture<gl::TEXTURE_2D, true>>& GetTextureManager();
}
