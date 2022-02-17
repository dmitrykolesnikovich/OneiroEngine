//
// Created by Dezlow on 15.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "API/Input.hpp"
#include "Runtime/Runtime.hpp"

bool oe::API::Input::Keyboard::isKeyPressed(oe::API::Input::Keyboard::Key key)
{
    if (glfwGetKey(Runtime::Engine::GetWindow(), key) == GLFW_PRESS)
        return true;
    return false;
}
bool oe::API::Input::Mouse::isButtonPressed(oe::API::Input::Mouse::Button button)
{
    if (glfwGetMouseButton(Runtime::Engine::GetWindow(), button) == GLFW_PRESS)
        return true;
    return false;
}
