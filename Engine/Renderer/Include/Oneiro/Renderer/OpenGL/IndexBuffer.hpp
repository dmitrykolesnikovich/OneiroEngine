//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_INDEXBUFFER_HPP
#define ONEIRO_INDEXBUFFER_HPP

#include "DZL/types.hpp"
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    namespace Renderer
    {
        class OE_API IndexBuffer
        {
        public:
            ~IndexBuffer();
            void Init(const dzl::uint32_t* data, dzl::uint32_t count);
            void Bind() const;
            void UnBind() const;
        private:
            dzl::uint32_t mID{};
        };
    }
}

#endif //ONEIRO_INDEXBUFFER_HPP
