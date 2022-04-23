//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Logger.hpp"

namespace oe::Core
{
	Root::Root()
	{
        mConfigsMap["user"] = new Config("user.cfg");
        mConfigsMap["renderer"] = new Config("renderer.cfg");
	}

    Root::~Root()
    {
	    mWindowInstance = nullptr;
	    mApplicationInstance = nullptr;
    }

    Window* Root::GetWindow() { return mWindowInstance; }

    Runtime::Application* Root::GetApplication() { return mApplicationInstance; }

    void Root::SetApplication(Runtime::Application* app) { if (!mApplicationInstance) mApplicationInstance = app; }

    void Root::SetWindow(Window* window) { if (!mWindowInstance) mWindowInstance = window; }

    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance{};
    std::unordered_map<std::string, Config*> Root::mConfigsMap;
}
