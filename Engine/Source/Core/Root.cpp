//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Renderer/OpenGL/Texture.hpp"

#include <chrono>
#include <future>

namespace oe::Core
{
    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance;
    ResourceManager<Renderer::Texture>* Root::mTextureManager;
    std::vector<std::future<void>> Root::mFutures;
    std::unordered_map<std::string, Config*> Root::mConfigsMap;
    std::string Root::mGLVersion;
    std::string Root::mGLSLVersion;

	Root::Root()
	{
		mTextureManager = new ResourceManager<Renderer::Texture>;
        mConfigsMap["user"] = new Config("user.cfg");
        mConfigsMap["renderer"] = new Config("renderer.cfg");
	}

    Root::~Root()
    {
	    mWindowInstance = nullptr;
	    mApplicationInstance = nullptr;
    }

    Window* Root::GetWindow() { return mWindowInstance; }

    Runtime::Application* Root::GetApplication() { return mApplicationInstance; }

    ResourceManager<Renderer::Texture>* Root::GetTextureManager() { return mTextureManager; }

    std::vector<std::future<void>>& Root::GetFutures() { return mFutures; }

    Config* Root::GetConfig(const std::string& name) { return mConfigsMap[name]; }

    const std::string& Root::GetGLVersion() { return mGLVersion; }

    const std::string& Root::GetGLSLVersion() { return mGLSLVersion; }

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
        uint64_t i{0};
        while (i != futuresSize)
        {
            if (mFutures[i]._Is_ready())
            {
                mTextureManager->Get(i)->Load(); i++;
            }
        }
    }

    void Root::SetApplication(Runtime::Application* app) { if (!mApplicationInstance) mApplicationInstance = app; }

    void Root::SetWindow(Window* window) { if (!mWindowInstance) mWindowInstance = window; }

    void Root::SetGLVersion(const std::string& version) { mGLVersion = version; }

    void Root::SetGLSLVersion(const std::string& version) { mGLSLVersion = version; }
}
