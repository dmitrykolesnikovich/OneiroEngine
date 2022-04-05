//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//


#pragma once

#include <iostream>
#include <Oneiro/Core/Window.hpp>
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Runtime
{
    class OE_API Engine
    {
    public:
        static void Init()
        {
            Core::Init();
            Renderer::Init();
        }

        static void Shutdown()
        {
            Renderer::Shutdown();
            Core::Shutdown();
        }

        static void Run(const std::shared_ptr<Application>& app);
    private:
        static void SetupCallbacks();
        static Core::Root* root;
        static Core::Window* window;
    };
}