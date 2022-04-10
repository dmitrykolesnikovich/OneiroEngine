//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "ResourceManager.hpp"

#include <future>

namespace oe::Core { class Window; }
namespace oe::Runtime { class Engine; class Application; }
namespace oe::Renderer { class Texture;  }

namespace oe::Core
{
    class Root
    {
    public:
        ~Root();
    public: // Getters
        Root();
        static Window* GetWindow();
        static Runtime::Application* GetApplication();
        static ResourceManager<Renderer::Texture>* GetTextureManager();
        static std::vector<std::future<void>>& GetFutures() { return mFutures; }
    public: // Setters (NOT STATIC!)
        void LoadTexturesAsync();
        void SetApplication(Runtime::Application* app);
        void SetWindow(Window* window);
    private:
        static std::vector<std::future<void>> mFutures;
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
        static ResourceManager<Renderer::Texture>* mTextureManager;
    };
}