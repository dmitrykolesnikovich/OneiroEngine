//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Core.hpp"

namespace oe::Core
{
    void Init()
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to init glfw!");
        Logger::Create("log", "log.txt");
    }

    void Shutdown() { glfwTerminate(); }
}