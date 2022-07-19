//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Input.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"

namespace oe::Input
{
    bool IsKey(Action action, Key key)
    {
        return glfwGetKey(Core::Root::GetWindow()->GetGLFW(), static_cast<int>(key)) == static_cast<int>(action);
    }

    bool IsMouseButton(Action action, Button button)
    {
        return glfwGetMouseButton(Core::Root::GetWindow()->GetGLFW(), static_cast<int>(button)) == static_cast<int>(action);
    }

    void SetMode(InputMode mode, InputValue value)
    {
        glfwSetInputMode(Core::Root::GetWindow()->GetGLFW(), mode, value);
    }

    int GetMode(InputMode mode)
    {
        return glfwGetInputMode(Core::Root::GetWindow()->GetGLFW(), mode);
    }
} // namespace oe::Input