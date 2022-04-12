//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"

#include <chrono>
#include <future>

#include "Oneiro/Renderer/OpenGL/Texture.hpp"

namespace oe::Core
{
    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance;
    ResourceManager<Renderer::Texture>* Root::mTextureManager;
    std::vector<std::future<void>> Root::mFutures;

	Root::Root() { mTextureManager = new ResourceManager<Renderer::Texture>; }

    Root::~Root()
    {
	    mWindowInstance = nullptr;
	    mApplicationInstance = nullptr;
    }

    Window* Root::GetWindow() { return mWindowInstance; }

    Runtime::Application* Root::GetApplication() { return mApplicationInstance; }

    ResourceManager<Renderer::Texture>* Root::GetTextureManager() { return mTextureManager; }

    void Root::LoadTexturesAsync()
    {
        using namespace std::chrono_literals;
        const auto& resources = mTextureManager->GetResources();
        for (const auto& resource : resources)
        {
            mFutures.emplace_back(std::async(std::launch::async,
                [](Renderer::Texture* texture) { texture->PreLoad(); }, resource.get()));
        }

        // TODO: optimize while loop
        const size_t futuresSize = mFutures.size();
        uint64_t i{};
        while (i != futuresSize)
        {
            if (mFutures[i]._Is_ready())
            {
                mTextureManager->GetItem(i)->Load(); i++;
            }
        }
    }

    void Root::SetApplication(Runtime::Application* app)
    {
	    if (!mApplicationInstance) mApplicationInstance = app;
    }

    void Root::SetWindow(Window* window)
    {
	    if (!mWindowInstance) mWindowInstance = window;
    }
}
