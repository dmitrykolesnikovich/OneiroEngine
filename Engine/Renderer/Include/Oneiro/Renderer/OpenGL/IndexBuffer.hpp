//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_INDEXBUFFER_HPP
#define ONEIRO_INDEXBUFFER_HPP

#define OE_DLL_EXPORT
#include <cstdint>

#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
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

#endif //ONEIRO_INDEXBUFFER_HPP
