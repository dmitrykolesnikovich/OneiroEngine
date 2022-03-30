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
    class OE_API VertexBuffer
    {
    public:
        ~VertexBuffer();
        void Create(uint32_t size, const float* pVertices);
        void Bind() const;
        void UnBind() const;

        static void PushLayout(uint32_t id, uint32_t size,
                               uint32_t stride, uint32_t p);
    private:
        uint32_t mID{};
    };
}