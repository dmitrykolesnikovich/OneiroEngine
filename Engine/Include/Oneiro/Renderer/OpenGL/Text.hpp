//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//
#pragma once

#include "Base.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "glm/glm.hpp"

#include <map>

namespace oe::Renderer::GL
{
    class Text
    {
        struct Character
        {
            GL::Texture<gl::TEXTURE_2D>* Texture = new GL::Texture<gl::TEXTURE_2D>;
            glm::ivec2 Size{};
            glm::ivec2 Bearing{};
            uint32_t Advance{};
        };

      public:
        Text() = default;
        Text(const Text&) = delete;

        void Init();

        void SetString(const std::string& str);

        void Draw(const glm::vec2& pos = {25.0f, 25.0f}, const glm::vec3& color = {1.0f, 0.0f, 0.0f});

      private:
        std::map<char, Character> mCharacters{};
        std::string mString{};
        Shader mShader{};
        VertexArray mVAO{};
        Buffer<gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW> mVBO{};
    };
} // namespace oe::Renderer::GL
