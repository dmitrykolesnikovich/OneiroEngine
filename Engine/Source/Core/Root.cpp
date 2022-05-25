//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Oneiro.hpp"
#include "Oneiro/Scene/Scene.hpp"
#include "Oneiro/Scene/SceneManager.hpp"

namespace oe::Core
{
    Root::Root()
    {
        if (mIsCreated) return;
        mSceneManager = std::make_unique<Scene::SceneManager>();
        mIsCreated = true;
    }

    Root::~Root()
    {
        mWindowInstance = nullptr;
        mApplicationInstance = nullptr;
    }

    Window* Root::GetWindow() { return mWindowInstance; }

    Runtime::Application* Root::GetApplication() { return mApplicationInstance; }

    Scene::SceneManager* Root::GetSceneManager()
    {
        return mSceneManager.get();
    }

    void Root::SetApplication(Runtime::Application* app)
    {
        if (mApplicationInstance)
        OE_THROW_ERROR("Root", "Failed to set application instance, because it's already set!")
        mApplicationInstance = app;
    }

    void Root::SetWindow(Window* window)
    {
        if (mWindowInstance)
        OE_THROW_ERROR("Root", "Failed to set window instance, because it's already set!")
        mWindowInstance = window;
    }
    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance{};
    std::unique_ptr<Scene::SceneManager> Root::mSceneManager{};
}
