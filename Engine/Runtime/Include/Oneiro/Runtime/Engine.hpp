//
// Created by Dezlow on 20.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_RUNTIME_APPLICATION_HPP
#define ONEIRO_RUNTIME_APPLICATION_HPP

#include <iostream>
#include "Oneiro/Core/Input.hpp"
#include "Application.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Logger.hpp"

namespace oe
{
    namespace Runtime
    {
        class OE_API Engine
        {
        public:
            static void Run(SDK::Application& app)
            {
                Core::Window window;
                Core::Logger::Create("log.txt");
                Renderer::Init();

                window.Create();

                app.Init();

                while (!window.isClosed())
                {
                    oe::Core::Window::PollEvents();
                    oe::Core::Window::WaitEvents();

                    app.Update();

                    window.SwapBuffers();
                }

                app.Close();

                Renderer::Shutdown();
            }
        };
    }
}

#endif //ONEIRO_RUNTIME_APPLICATION_HPP
