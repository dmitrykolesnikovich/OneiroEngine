//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_VERTEXBUFFER_HPP
#define ONEIRO_VERTEXBUFFER_HPP

#include "DZL/types.hpp"
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    class OE_API VertexBuffer
    {
    public:
        ~VertexBuffer();
        void Create(dzl::uint32_t size, const float* pVertices);
        void Bind() const;
        void UnBind() const;

        static void PushLayout(dzl::uint32_t id, dzl::uint32_t size,
                               dzl::uint32_t stride, dzl::uint32_t p);
    private:
        dzl::uint32_t mID{};
    };
}

#endif //ONEIRO_VERTEXBUFFER_HPP
