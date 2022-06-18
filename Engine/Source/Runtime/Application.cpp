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

	bool Application::Init()
	{
		return true;
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
