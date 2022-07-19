//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"

namespace oe::Core
{
    Window* Root::GetWindow()
    {
        return mWindowInstance;
    }

    Runtime::Application* Root::GetApplication()
    {
        return mApplicationInstance;
    }

    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance{};
} // namespace oe::Core
