//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <string>
#include "Oneiro/Runtime/Engine.hpp"
#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Runtime
{
    void Engine::Init()
    {
        Core::Init();
        Renderer::Init();
        mRoot = new Core::Root;
        mWindow = new Core::Window;
    }

    void Engine::Run(const std::shared_ptr<Application>& app)
    {
        mRoot->SetApplication(app.get());
        mRoot->SetWindow(mWindow);

        Core::Window::SetErrorCallback([](int error, const char* description){
            Logger::Get("log")->PrintError("GLFW ERROR[" + std::to_string(error) + "]: " + description);
        });

        if (!mWindow->Create())
            throw std::runtime_error("Failed to create window!");

        Core::Window::SetFramerate(1);
        SetupCallbacks();

        if (!app->Init())
            throw std::runtime_error("Failed to initialize application!");

        while (!mWindow->IsClosed())
        {
            if (app->IsStopped())
                break;

            Core::Window::PollEvents();

            if (!app->Update())
                break;

            mWindow->SwapBuffers();
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
        Core::Window::SetFrameBufferSizeCallback([](int width, int height){
            Renderer::Viewport(width, height);
        });

        Core::Window::SetFocusCallback([](bool isFocused){
            if (isFocused)
                Core::Window::SetFramerate(1); // 60 fps
            else
                Core::Window::SetFramerate(-3); // 15 fps
        });
    }

    void Engine::Shutdown()
    {
        delete mWindow;
        delete mRoot;

        Renderer::Shutdown();
        Core::Shutdown();
    }

    Core::Root* Engine::mRoot{};
    Core::Window* Engine::mWindow{};
}