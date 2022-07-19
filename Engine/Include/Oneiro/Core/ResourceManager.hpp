//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "OpenGL/gl_core_4_5.hpp"

namespace oe::Renderer::GL
{
    class Mesh;

    template <int TextureType, bool Mipmaps> class Texture;
    class TextureData;
} // namespace oe::Renderer::GL

namespace oe::Core
{
    template <class T, class... Args> class ResourceManager
    {
      public:
        T* Add(const std::shared_ptr<T>& resourcePtr, const Args&... args)
        {
            mResources.push_back(std::make_pair(resourcePtr, std::make_tuple(std::move(args)...)));
            return resourcePtr.get();
        }

        [[nodiscard]] constexpr T* Get(size_t index) const
        {
            return mResources[index].first.get();
        }
        [[nodiscard]] constexpr size_t Size() const
        {
            return mResources.size();
        }
        [[nodiscard]] constexpr std::vector<std::pair<std::shared_ptr<T>, std::tuple<Args...>>>& GetResources()
        {
            return mResources;
        }

      private:
        std::vector<std::pair<std::shared_ptr<T>, std::tuple<Args...>>> mResources{};
    };

    void LoadResources();

    ResourceManager<Renderer::GL::Texture<gl::TEXTURE_2D, true>>& GetTexturesManager();
    ResourceManager<Renderer::GL::Mesh, const std::string>& GetMeshesManager();
} // namespace oe::Core
