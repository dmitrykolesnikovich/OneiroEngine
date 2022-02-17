//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#ifndef ENGINE_APPLICATION_HPP
#define ENGINE_APPLICATION_HPP

#include <memory>
#include "Core/Window.hpp"
#include "API/Application.hpp"

namespace oe
{
    namespace Runtime
    {
        class Engine
        {
        public:
            static void Run();
            static GLFWwindow* GetWindow() { return mWindow.GetWindow(); }
        private:
            static Core::Window mWindow;
            static std::shared_ptr<API::Application> mApplication;
        };

        std::shared_ptr<API::Application> CreateApplication();
    }
}

#endif //ENGINE_APPLICATION_HPP
