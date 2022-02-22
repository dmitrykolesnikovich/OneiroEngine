//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_VERTEXARRAY_HPP
#define ONEIRO_VERTEXARRAY_HPP

#include "DZL/types.hpp"
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    class OE_API VertexArray
    {
    public:
        ~VertexArray();
        void Init();
        void Bind() const;
        void UnBind() const;
    private:
        dzl::uint32_t mID{};
    };
}

#endif //ONEIRO_VERTEXARRAY_HPP
