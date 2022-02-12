//
// Created by Dezlow on 12.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ENGINE_ENTRYPOINT_HPP
#define ENGINE_ENTRYPOINT_HPP

#include <iostream>
#include "Application.hpp"
#include "Core/Core.hpp"
#include "Debugger/Debugger.hpp"
#include "Renderer/Renderer.hpp"

int main(int argc, char** argv)
{
    using namespace oe;
    try
    {
        Core::Init();
        Debugger::Init();
        Renderer::Init();

        auto app = oe::Runtime::CreateApplication();
        app->Run();

        Renderer::Shutdown();
        Debugger::Shutdown();
        Core::Shutdown();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }
}

#endif //ENGINE_ENTRYPOINT_HPP
