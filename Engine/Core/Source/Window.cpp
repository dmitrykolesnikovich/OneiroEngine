//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Window.hpp"

namespace oe::Core
{
    Window* Window::mInstance{};
    Window::WindowCallbacks Window::mCallbacks{};
}