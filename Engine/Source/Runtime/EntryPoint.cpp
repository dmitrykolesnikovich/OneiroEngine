//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/EntryPoint.hpp"

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Runtime/Engine.hpp"

int main(const int argc, char* argv[])
{
    using namespace oe::Runtime;
    using namespace oe;

    const auto app = CreateApplication(argc, argv);

    try
    {
        Engine::Init();
        Engine::Run(app);
    }
    catch (const std::exception& ex)
    {
        log::get("log")->error(ex.what());
    }
    app.~shared_ptr();
    Engine::Shutdown();
    return 0;
}
