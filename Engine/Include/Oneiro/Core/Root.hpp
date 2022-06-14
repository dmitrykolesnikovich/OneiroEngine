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
    class Config;
}

namespace oe::Runtime
{
    class Application;
}

namespace oe::World
{
    class World;
    class WorldManager;
}

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
        static World::WorldManager* GetWorldManager();
    public: // Setters | Non static to protect against unexpected behavior
        void SetApplication(Runtime::Application* app);
        void SetWindow(Window* window);
    private:
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
        static std::unique_ptr<World::WorldManager> mWorldManager;
        inline static bool mIsCreated{};
    };
}
