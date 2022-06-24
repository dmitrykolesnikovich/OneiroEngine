//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Runtime
{
	class Engine
	{
	public:
		static void Init();
		static void Run(const std::shared_ptr<Application>& app);
		static void Shutdown();
		static float GetDeltaTime() { return mDeltaTime; }
	private:
		static bool UpdateGame(const std::shared_ptr<Application>& app, float deltaTime, const Core::Window* window);
		static void SetupEvents();
		inline static float mDeltaTime{};
	};
}
