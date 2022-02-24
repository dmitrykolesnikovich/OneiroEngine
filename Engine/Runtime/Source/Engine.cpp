//
// Created by Dezlow on 20.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//

#include "Oneiro/Runtime/Engine.hpp"

void oe::Runtime::Engine::Run(oe::Runtime::Application& app)
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