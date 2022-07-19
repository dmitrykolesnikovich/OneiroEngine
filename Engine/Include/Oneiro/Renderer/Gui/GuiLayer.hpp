//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "imgui.h"

namespace oe::Renderer::GuiLayer
{
    using namespace ImGui;

    void PreInit();
    void Init();
    void NewFrame();
    void Draw();
    void Shutdown();
} // namespace oe::Renderer::GuiLayer
