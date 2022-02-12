//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ENGINE_CORE_WINDOW_HPP
#define ENGINE_CORE_WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "DZL/string.hpp"

namespace oe
{
    namespace Core
    {
        class Window
        {
        public:
            bool Create(dzl::uint16_t width, dzl::uint16_t height, const dzl::string& title);
            bool isClosed();

            void PollEvents();
            void Update();

            GLFWwindow* GetWindow() { return mWindow; }
            ~Window();
        private:
            GLFWwindow* mWindow;
        };
    }
}

#endif //ENGINE_CORE_WINDOW_HPP
