//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Engine.hpp"
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Logger.hpp"

int main(int argc, char* argv[])
{
    oe::Runtime::Engine::Init();
    try
    {
        auto app = oe::Runtime::CreateApplication(argc, argv);
        oe::Runtime::Engine::Run(app);
    }
    catch (const std::exception& ex)
    {
        oe::Logger::Get("log")->PrintError(std::string("Exception: ") + ex.what());
    }
    oe::Runtime::Engine::Shutdown();
}
