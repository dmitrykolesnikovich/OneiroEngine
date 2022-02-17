//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include <OpenGL/gl_core_4_5.hpp>
#include "Runtime/Runtime.hpp"

oe::Core::Window oe::Runtime::Engine::mWindow{};
std::shared_ptr<oe::API::Application> oe::Runtime::Engine::mApplication{};

void oe::Runtime::Engine::Run()
{
    mApplication = Runtime::CreateApplication();
    mWindow.Create(1280, 720, "Oneiro Engine");

    mApplication->Init();

    while (!mWindow.isClosed())
    {
        mWindow.PollEvents();
        glfwWaitEvents();

        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        mApplication->Update();

        mWindow.Update();
    }
    mApplication->Close();
}
