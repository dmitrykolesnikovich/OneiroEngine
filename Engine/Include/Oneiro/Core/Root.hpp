//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace oe::Core
{
	class Window;
}

namespace oe::Runtime
{
	class Application;
	class Engine;
}

namespace oe::Core
{
	class Root
	{
	public: // Getters
		static Window* GetWindow();
		static Runtime::Application* GetApplication();
	private:
		friend class Runtime::Engine;
	private:
		static Window* mWindowInstance;
		static Runtime::Application* mApplicationInstance;
	};
}
