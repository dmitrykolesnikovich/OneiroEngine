//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Engine.hpp"

namespace oe::Runtime
{
    void Engine::Run(const std::shared_ptr<Application>& app)
    {
        root = new Core::Root;
        window = new Core::Window;

        root->SetApplication(app.get());

        root->SetWindow(window);

        Core::Window::SetErrorCallback([](int error, const char* description){
            Logger::Get("log")->PrintError("GLFW ERROR[" + std::to_string(error) + "]: " + description);
        });

        if (!window->Create())
            throw std::runtime_error("Failed to create window!");

        Core::Window::SetFramerate(1);
        SetupCallbacks();

        if (!app->Init())
            throw std::runtime_error("Failed to initialize application!");

        while (!window->IsClosed())
        {
            if (app->IsStopped())
                break;

            Core::Window::PollEvents();

            if (!app->Update())
                break;

            window->SwapBuffers();
        }

        app->Shutdown();
    }

    void Engine::SetupCallbacks()
    {
        Core::Window::SetKeyCallback([](Input::Key key, Input::Action action){
            Core::Root::GetApplication()->HandleKey(key, action);
        });
        Core::Window::SetMouseButtonCallback([](Input::Button button, Input::Action action){
            Core::Root::GetApplication()->HandleButton(button, action);
        });
        Core::Window::SetFrameBufferSizeCallback([](int w, int h){
            gl::Viewport(0, 0, w, h);
        });

        Core::Window::SetFocusCallback([](bool isFocused){
            if (isFocused)
                Core::Window::SetFramerate(1); // 60 fps
            else
                Core::Window::SetFramerate(-3); // 15 fps
        });
    }

    Core::Root* Engine::root{};
    Core::Window* Engine::window{};
}