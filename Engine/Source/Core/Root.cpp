//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Scene/Scene.hpp"
#include "Oneiro/Scene/SceneManager.hpp"

namespace oe::Core
{
	Root::Root()
	{
		mConfigsMap["user"] = new Config("user.cfg");
		mConfigsMap["renderer"] = new Config("renderer.cfg");
		mScene = new Scene::Scene;
		mSceneManager = new Scene::SceneManager(mScene);
	}

	Root::~Root()
	{
		mWindowInstance = nullptr;
		mApplicationInstance = nullptr;
	}

	Window* Root::GetWindow() { return mWindowInstance; }

	Runtime::Application* Root::GetApplication() { return mApplicationInstance; }

	Scene::Scene* Root::GetScene()
	{
		return mScene;
	}

	void Root::LoadScene(const std::string& filePath)
	{
		if (!mSceneManager->Load(filePath))
			OE_THROW_ERROR("Scene", "Failed to load scene: " + filePath)
	}

	void Root::SetApplication(Runtime::Application* app) { if (!mApplicationInstance) mApplicationInstance = app; }

	void Root::SetWindow(Window* window) { if (!mWindowInstance) mWindowInstance = window; }

	Scene::SceneManager* Root::GetSceneManager() { return mSceneManager; }

	Window* Root::mWindowInstance{};
	Runtime::Application* Root::mApplicationInstance{};
	std::unordered_map<std::string, Config*> Root::mConfigsMap;
	Scene::Scene* Root::mScene{};
	Scene::SceneManager* Root::mSceneManager{};
}
