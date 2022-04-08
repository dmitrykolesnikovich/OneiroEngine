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

	bool Application::Init() { return true; }

	bool Application::Update() { return true; }
	
	void Application::Shutdown() {}

	void Application::HandleKey(Input::Key key, Input::Action action) {}

	void Application::HandleButton(Input::Button button, Input::Action action) {}

	void Application::SetKeyInput(bool isEnabled) { Core::Root::GetWindow()->IsKeyInput = isEnabled; }
	void Application::SetButtonInput(bool isEnabled) { Core::Root::GetWindow()->IsMouseButtonInput = isEnabled; }
}