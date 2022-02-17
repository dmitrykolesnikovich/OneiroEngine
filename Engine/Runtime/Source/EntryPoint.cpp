//
// Created by Dezlow on 14.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Runtime/Runtime.hpp"
#include "Runtime/EntryPoint.hpp"
#include "Core/Core.hpp"
#include "Debugger/Debugger.hpp"
#include "Renderer/Renderer.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    using namespace oe;
    Core::Init();
    Debugger::Init();
    Renderer::Init();
    try
    {
        Runtime::Engine::Run();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << '\n';
    }

    Renderer::Shutdown();
    Debugger::Shutdown();
    Core::Shutdown();
}