//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <OpenGL/gl_core_4_5.hpp>
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe { class VertexArray; }

namespace oe::Renderer
{
    OE_API void Init();
    OE_API void Shutdown();
}