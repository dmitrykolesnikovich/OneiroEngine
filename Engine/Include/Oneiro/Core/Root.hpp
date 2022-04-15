//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "ResourceManager.hpp"

#include <future>

namespace oe::Core { class Window; class Config; }
namespace oe::Runtime { class Engine; class Application; }
namespace oe::Renderer { class Texture;  }

namespace oe::Core
{
    class Root
    {
    public:
        Root();
        ~Root();
    public: // Getters
        static Window* GetWindow();
        static Runtime::Application* GetApplication();
        static ResourceManager<Renderer::Texture>* GetTextureManager();
        static std::vector<std::future<void>>& GetFutures();
        static Config* GetConfig(const std::string& name);
        static const std::string& GetGLVersion();
        static const std::string& GetGLSLVersion();
    public: // Setters (NOT STATIC!)
        void LoadTexturesAsync();
        void SetApplication(Runtime::Application* app);
        void SetWindow(Window* window);
        void SetGLVersion(const std::string& version);
        void SetGLSLVersion(const std::string& version);
    private:
        static std::vector<std::future<void>> mFutures;
        static ResourceManager<Renderer::Texture>* mTextureManager;
        static std::unordered_map<std::string, Config*> mConfigsMap;
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
        static std::string mGLVersion;
        static std::string mGLSLVersion;
    };
}