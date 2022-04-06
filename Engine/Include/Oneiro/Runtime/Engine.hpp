//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//


#pragma once

#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Runtime
{
    class OE_API Engine
    {
    public:
        static void Init();
        static void Run(const std::shared_ptr<Application>& app);
        static void Shutdown();
    private:
        static void SetupCallbacks();
        static Core::Window* mWindow;
        static Core::Root* mRoot;
    };
}