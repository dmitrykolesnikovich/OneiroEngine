//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Base.hpp"

namespace oe::Renderer::GL
{
    class VertexArray
    {
      public:
        VertexArray() = default;
        VertexArray(const VertexArray&) = delete;
        ~VertexArray();

        void Generate();

        void Bind() const;

        static void UnBind();

        [[nodiscard]] uint32_t Get() const;

      private:
        uint32_t mID{};
    };
} // namespace oe::Renderer::GL
