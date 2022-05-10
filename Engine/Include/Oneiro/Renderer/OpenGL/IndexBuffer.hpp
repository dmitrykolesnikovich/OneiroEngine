//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cstdint>

namespace oe::Renderer::GL
{
    class IndexBuffer
    {
    public:
        ~IndexBuffer();
        void Init(const void* data, int64_t count);
        void Bind() const;
        void UnBind() const;
    private:
        uint32_t mID{};
    };
}