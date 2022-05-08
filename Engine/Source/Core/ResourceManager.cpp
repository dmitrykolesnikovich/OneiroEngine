//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/ResourceManager.hpp"
#include "Oneiro/Renderer/OpenGL/Texture.hpp"

#include <future>
#include <chrono>
#include <deque>

std::deque<std::future<void>> futures{};
oe::Core::ResourceManager<oe::Renderer::Texture> textureManager;
// TODO: add async loading

namespace oe::Core
{

    void LoadResources()
    {
        using namespace std::chrono_literals;
        {
            // Load textures
            {
                const auto& textures = textureManager.GetResources();
                size_t texturesSize = textures.size();
                for (size_t i{}; i < texturesSize; ++i)
                {
                    futures.emplace_back(std::async(std::launch::async, [](Renderer::Texture* texture) { texture->PreLoad(); }, textures[i].get()));
                }


                size_t futuresSize = futures.size();
                size_t i{};
                while (i != futuresSize)
                {
                    if (futures[i].wait_for(0ms) == std::future_status::ready)
                    {
                        textures[i]->Load();
                        i++;
                    }
                }
            }
        }
    }

    ResourceManager<Renderer::Texture>& GetTextureManager()
    {
        return textureManager;
    }
}