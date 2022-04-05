//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/EntryPoint.hpp"

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
        return 1;
    }
    oe::Runtime::Engine::Shutdown();
    return 0;
}