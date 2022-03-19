//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_VERTEXBUFFER_HPP
#define ONEIRO_VERTEXBUFFER_HPP

#define OE_DLL_EXPORT
#include <cstdint>

#include "Oneiro/Core/Oneiro.hpp"

namespace oe
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

#endif //ONEIRO_VERTEXBUFFER_HPP
