//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"

namespace oe::Core
{
    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance;

    Root::~Root()
    {
	    mWindowInstance = nullptr;
	    mApplicationInstance = nullptr;
    }

    Window* Root::GetWindow()
    { return mWindowInstance; }

    Runtime::Application* Root::GetApplication()
    { return mApplicationInstance; }

    void Root::SetApplication(Runtime::Application* app)
    {
	    if (!mApplicationInstance) mApplicationInstance = app;
    }

    void Root::SetWindow(Window* window)
    {
	    if (!mWindowInstance) mWindowInstance = window;
    }
}
