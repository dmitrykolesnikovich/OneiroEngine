//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Application.hpp"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"

namespace oe::Runtime
{
	Application::~Application() = default;

	Application::Application(const char* name, int width, int height) {
		mWindow = std::make_shared<Core::Window>(name, width, height);
	}

	bool Application::Init()
	{
		return true;
	}

	Core::Window* Application::GetWindow() const
    {
		return mWindow.get();
	}

	bool Application::Update(float deltaTime)
	{
		return true;
	}

	void Application::Shutdown()
	{
	}

	void Application::HandleKey(Input::Key key, Input::Action action)
	{
	}

	void Application::HandleButton(Input::Button button, Input::Action action)
	{
	}

	void Application::MousePos(double xPos, double yPos)
	{
	}

	bool Application::IsStopped() const
	{
		return mIsStopped;
	}

	void Application::Stop()
	{
		mIsStopped = true;
	}
}
