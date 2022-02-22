//
// Created by Dezlow on 20.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_CORE_HPP
#define ONEIRO_CORE_HPP

#include "Logger.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace oe
{
    namespace Core
    {
        void Init() { glfwInit(); Logger::Create("log.txt"); }
        void Shutdown() { glfwTerminate(); }
    }
}

#endif //ONEIRO_CORE_HPP
