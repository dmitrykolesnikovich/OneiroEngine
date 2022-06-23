//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Core/Input.hpp"
#include <memory>

namespace oe::Runtime
{
	class Application
	{
	public:
		virtual ~Application();
		Application(const char* name, int width, int height);
		virtual bool Init();
		virtual bool Update(float deltaTime);
		virtual void Shutdown();
		virtual void HandleKey(Input::Key key, Input::Action action);
		virtual void HandleButton(Input::Button button, Input::Action action);
		virtual void MousePos(double xPos, double yPos);

	    [[nodiscard]] Core::Window* GetWindow() const;
		[[nodiscard]] bool IsStopped() const;
	protected:
		void Stop();
	private:
		bool mIsStopped{};
		std::shared_ptr<Core::Window> mWindow;
	};

	std::shared_ptr<Application> CreateApplication(int argc, char* argv[]);
}
