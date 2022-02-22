//
// Created by Dezlow on 21.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef SANDBOX_INPUT_HPP
#define SANDBOX_INPUT_HPP

#include "Window.hpp"

namespace oe
{
    namespace Input
    {
        enum Key
        {
            Q = GLFW_KEY_Q,
            W = GLFW_KEY_W,
            E = GLFW_KEY_E,
            R = GLFW_KEY_R,
            T = GLFW_KEY_T,
            Y = GLFW_KEY_Y,
            U = GLFW_KEY_U,
            I = GLFW_KEY_I,
            O = GLFW_KEY_O,
            P = GLFW_KEY_P,
            A = GLFW_KEY_A,
            S = GLFW_KEY_S,
            D = GLFW_KEY_D,
            F = GLFW_KEY_F,
            G = GLFW_KEY_G,
            H = GLFW_KEY_H,
            J = GLFW_KEY_J,
            K = GLFW_KEY_K,
            L = GLFW_KEY_L,
            Z = GLFW_KEY_Z,
            X = GLFW_KEY_X,
            C = GLFW_KEY_C,
            V = GLFW_KEY_V,
            B = GLFW_KEY_B,
            N = GLFW_KEY_N,
            M = GLFW_KEY_M
        };

        enum Type
        {
            PRESS = GLFW_PRESS,
            RELEASE = GLFW_RELEASE
        };

        enum Button
        {
            LEFT = GLFW_MOUSE_BUTTON_LEFT,
            RIGHT = GLFW_MOUSE_BUTTON_RIGHT
        };

        bool GetKey(oe::Input::Key key, oe::Input::Type type) { return glfwGetKey(Core::Window::Get()->GetGLFWWindow(), key) == type; }
        bool GetMouseButton(oe::Input::Button button, oe::Input::Type type) { return glfwGetMouseButton(Core::Window::Get()->GetGLFWWindow(), button) == type; }

        class Command
        {
        public:
            virtual ~Command() = default;
            virtual void execute() = 0;
            virtual void undo() = 0;
        };
    }
}

#endif //SANDBOX_INPUT_HPP
