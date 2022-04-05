//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

namespace oe::Core { class Window; }
namespace oe::Runtime { class Engine; class Application; }

namespace oe::Core
{
    class Root
    {
    public: // Getters
        static Window* GetWindow() { return mWindowInstance; }
        static Runtime::Application* GetApplication() { return mApplicationInstance; }
    public: // Setters (NOT STATIC!)
        void SetApplication(Runtime::Application* app) { if (!mApplicationInstance) mApplicationInstance = app; }
        void SetWindow(Window* window) { if (!mWindowInstance) mWindowInstance = window; }
    private:
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
    };
}