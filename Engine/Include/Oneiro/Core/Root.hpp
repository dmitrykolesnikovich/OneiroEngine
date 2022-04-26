//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <unordered_map>

namespace oe::Core
{
	class Window;
	class Config;
}

namespace oe::Runtime
{
	class Engine;
	class Application;
}

namespace oe::Scene
{
	class Scene;
	class SceneManager;
}

namespace oe::Core
{
	class Root
	{
	public:
		Root();
		~Root();
	public: // Getters
		static Window* GetWindow();
		static Runtime::Application* GetApplication();
		static Scene::Scene* GetScene();
		static void LoadScene(const std::string& filePath);
	public: // Setters (NOT STATIC!)
		void SetApplication(Runtime::Application* app);
		void SetWindow(Window* window);
	public: // Getters (NON STATIC!)
		Scene::SceneManager* GetSceneManager();
	private:
		static std::unordered_map<std::string, Config*> mConfigsMap;
		static Window* mWindowInstance;
		static Runtime::Application* mApplicationInstance;
		static Scene::Scene* mScene;
		static Scene::SceneManager* mSceneManager;
	};
}
