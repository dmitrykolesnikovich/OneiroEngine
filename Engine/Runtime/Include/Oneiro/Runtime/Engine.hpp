//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//


#pragma once

#include <iostream>
#include <Oneiro/Core/Window.hpp>
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Core.hpp"

namespace oe::Runtime
{
    class OE_API Engine
    {
    public:
        static void Init()
        {
            Core::Init();
            Renderer::Init();
        }

        static void Shutdown()
        {
            Renderer::Shutdown();
            Core::Shutdown();
        }

        static void Run(const std::shared_ptr<Application>& app)
        {
            mApplication = app.get();

            if (!mWindow->Create())
                throw std::runtime_error("Failed to create window!");

            mWindow->SetKeyCallback(Engine::KeyCallback);
            mWindow->SetMouseButtonCallback(Engine::MouseButtonCallback);
            mWindow->SetFrameBufferSizeCallback([](int w, int h){
                gl::Viewport(0, 0, w, h);
            });

            app->Init();

            while (!mWindow->IsClosed())
            {
                if (app->IsStopped())
                    break;

                Core::Window::PollEvents();

                if (!app->Update())
                    break;

                mWindow->SwapBuffers();
            }

            app->Close();
        }

        static Application* GetApplication() { return mApplication; }

    private:
        static void KeyCallback(Input::Key key, Input::Action action)
        {
            mApplication->HandleKey(key, action);
        }

        static void MouseButtonCallback(Input::Button button, Input::Action action)
        {
            mApplication->HandleButton(button, action);
        }

        static Application* mApplication;
        static Core::Window* mWindow;
    };
}

oe::Core::Window* oe::Runtime::Engine::mWindow{new oe::Core::Window};
oe::Runtime::Application* oe::Runtime::Engine::mApplication{nullptr};