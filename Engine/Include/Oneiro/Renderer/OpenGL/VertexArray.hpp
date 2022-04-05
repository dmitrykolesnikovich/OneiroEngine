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
    class OE_API VertexArray
    {
    public:
        ~VertexArray();
        void Init();
        void Bind() const;
        void UnBind() const;
    private:
        uint32_t mID{};
    };
}