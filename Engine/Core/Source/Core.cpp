//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Core/Core.hpp"
#include "GLFW/glfw3.h"

bool oe::Core::Init()
{
    glfwInit();

    return true;
}

void oe::Core::Shutdown()
{
    glfwTerminate();
}