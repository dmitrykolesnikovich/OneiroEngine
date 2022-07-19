//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include "Mesh.hpp"

namespace oe::Renderer::GL
{
    class Model
    {
      public:
        Model() = default;
        Model(const Model&) = delete;

        void Load(const std::string& path);
        void Load(const std::vector<Vertex>& vertices);

        void Draw() const;

        [[nodiscard]] std::string GetPath();
        [[nodiscard]] bool IsNeed2SaveVertices() const
        {
            return mIsNeed2SaveVertices;
        }
        [[nodiscard]] constexpr std::vector<Vertex>& GetVertices() const
        {
            return mMesh->GetVertices();
        }

      private:
        Mesh* mMesh{};

        std::string mPath{};

        VertexArray mVAO{};
        Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW> mVBO{};
        Buffer<gl::ELEMENT_ARRAY_BUFFER, gl::STATIC_DRAW> mEBO{};
        bool mIsNeed2SaveVertices{};
    };
} // namespace oe::Renderer::GL
