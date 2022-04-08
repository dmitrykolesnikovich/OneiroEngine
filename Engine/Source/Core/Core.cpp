//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <stdexcept>
#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Core/Logger.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace oe::Core
{
    void Init()
    {
        glfwSetErrorCallback([](int error, const char* description)
            {
                Event::Dispatcher::Post(Event::ErrorEvent(error, description));
            });
        Logger::Create("log", "log.txt");
        if (!glfwInit())
            throw std::runtime_error("Failed to init glfw!");
    }

    void Shutdown() { glfwTerminate(); }
}
