//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ENGINE_VERTEXARRAY_HPP
#define ENGINE_VERTEXARRAY_HPP

#include "DZL/types.hpp"

namespace oe
{
    namespace Renderer
    {
        class VertexArray
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
}

#endif //ENGINE_VERTEXARRAY_HPP
