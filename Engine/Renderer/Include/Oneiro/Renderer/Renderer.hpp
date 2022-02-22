//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_RENDERER_HPP
#define ONEIRO_RENDERER_HPP

#include <OpenGL/gl_core_4_5.hpp>
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    namespace Renderer
    {
        OE_API void Init();
        OE_API void Shutdown();
    }
}

#endif //ONEIRO_RENDERER_HPP
