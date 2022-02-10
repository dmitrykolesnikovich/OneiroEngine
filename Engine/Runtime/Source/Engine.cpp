//
// Created by Dezlow on 10.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Runtime/Engine.hpp"
#include "Runtime/Application.hpp"
#include "Core/Core.hpp"
#include "Debugger/Debugger.hpp"

void oe::runtime::Engine::Run()
{
    Debugger::Init();
    Debugger::GetLogger()->PrintMessage("Initializing engine...");
    Core::Init();

    auto app = Application();
    app.Init();
    app.Run();

    Debugger::GetLogger()->PrintMessage("Shutdown engine...");
    Core::Shutdown();
    Debugger::Shutdown();
}
