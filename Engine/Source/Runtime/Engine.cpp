//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Engine.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Scene/SceneManager.hpp"

namespace oe::Runtime
{
	void Engine::Init()
	{
		Core::Init();
		mRoot = new Core::Root;
		mWindow = new Core::Window;
	}

	void Engine::Run(const std::shared_ptr<Application>& app)
	{
		using namespace Core;
		mRoot->SetApplication(app.get());
		mRoot->SetWindow(mWindow);

		Event::Dispatcher::Subscribe<Event::ErrorEvent>([](const Event::Base& e)
		{
			const auto& errorEvent = dynamic_cast<const Event::ErrorEvent&>(e);
			log::get("log")->error("GLFW ERROR[" + std::to_string(errorEvent.Error) + "]: " +
				errorEvent.Description);
		});
		if (!mWindow->Create())
			throw std::runtime_error("Failed to create window!");

		SetupEvents();

		Renderer::Vulkan::PreInit();

		mRoot->LoadScene("Scenes/main.oescene");

		if (!app->Init())
			throw std::runtime_error("Failed to initialize application!");

		Renderer::Vulkan::Init();

		while (!mWindow->IsClosed())
		{
			if (app->IsStopped())
				break;

			Window::PollEvents();

			if (!app->Update())
				break;
		}

		mRoot->GetSceneManager()->Save("Scenes/main.oescene", "Main");
	}

	void Engine::Shutdown()
	{
		delete mWindow;
		delete mRoot;

		Renderer::Vulkan::Shutdown();
		Core::Shutdown();
	}

	void Engine::SetupEvents()
	{
		using namespace Core;
		Event::Dispatcher::Subscribe<Event::FrameBufferSizeEvent>([](const Event::Base& e)
		{
			const auto& resizeEvent = dynamic_cast<const Event::FrameBufferSizeEvent&>(e);
			if (resizeEvent.Width > 0 || resizeEvent.Height > 0)
				Window::UpdateSize(resizeEvent.Width, resizeEvent.Height);
		});

		Event::Dispatcher::Subscribe<Event::KeyInputEvent>([](const Event::Base& e)
		{
			const auto& keyInputEvent = dynamic_cast<const Event::KeyInputEvent&>(e);
			Root::GetApplication()->HandleKey(static_cast<Input::Key>(keyInputEvent.Key),
			                                  static_cast<Input::Action>(keyInputEvent.Action));
		});

		Event::Dispatcher::Subscribe<Event::MouseButtonEvent>([](const Event::Base& e)
		{
			const auto& mouseButtonEvent = dynamic_cast<const Event::MouseButtonEvent&>(e);
			Root::GetApplication()->HandleButton(static_cast<Input::Button>(mouseButtonEvent.Button),
			                                     static_cast<Input::Action>(mouseButtonEvent.Action));
		});
	}

	Core::Root* Engine::mRoot{};
	Core::Window* Engine::mWindow{};
}
