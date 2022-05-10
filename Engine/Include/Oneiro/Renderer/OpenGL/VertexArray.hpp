//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cstdint>

namespace oe::Renderer::GL
{
    class VertexArray
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