//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT
#include <cstdint>

#include "Oneiro/Core/Oneiro.hpp"

namespace oe::Renderer
{
    /**
     * Index buffer implementation for opengl rendering
     */
    class OE_API IndexBuffer
    {
    public:
        ~IndexBuffer();
        void Init(const uint32_t* data, uint32_t count);
        void Bind() const;
        void UnBind() const;
    private:
        uint32_t mID{};
    };
}