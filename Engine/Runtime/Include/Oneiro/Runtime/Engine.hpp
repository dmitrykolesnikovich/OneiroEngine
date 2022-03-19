//
// Created by Dezlow on 20.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_RUNTIME_ENGINE_HPP
#define ONEIRO_RUNTIME_ENGINE_HPP

#include <iostream>
#include "Oneiro/Core/Input.hpp"
#include "Application.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Core.hpp"

namespace oe
{
    namespace Runtime
    {
        class OE_API Engine
        {
        public:
            static void Run(Application& app)
            {
                Core::Init();
                Renderer::Init();

                mWindow->Create();

                app.Init();

                while (!mWindow->isClosed())
                {
                    Window::PollEvents();
                    Window::WaitEvents();

                    app.Update();

                    mWindow->SwapBuffers();
                }

                app.Close();

                Renderer::Shutdown();
                Core::Shutdown();
            }

        private:
            static Window* mWindow;
        };
    }
}

oe::Window* oe::Runtime::Engine::mWindow{new Window};

#endif //ONEIRO_RUNTIME_ENGINE_HPP