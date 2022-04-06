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
    public:
        ~Root();
    public: // Getters
        static Window* GetWindow();
        static Runtime::Application* GetApplication();
    public: // Setters (NOT STATIC!)
        void SetApplication(Runtime::Application* app);
        void SetWindow(Window* window);
    private:
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
    };
}