//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Runtime/Application.hpp"

void oe::runtime::Application::Init()
{
    mWindow.Create(1280, 720, "Oneiro Engine");
}

void oe::runtime::Application::Run()
{
    while (!mWindow.isClosed())
    {
        mWindow.PollEvents();

        mWindow.Update();
    }
}
