//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Renderer/OpenGL/Base.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace oe::Renderer
{
    class Sprite2D
    {
    public:
        void Init(bool keepAspectRatio = true);

        bool Load(const std::string& path);

        bool Load();;

        bool UnLoad();

        void Draw() const;

        void Move(glm::vec2 pos);

        void Scale(glm::vec2 scale);
    private:
        glm::mat4 mModel{1.0f};
        Renderer::Texture mTexture{};
        Renderer::Shader mShader{};
        Renderer::VertexBuffer mVBO;
        Renderer::VertexArray mVAO;
        float mAlpha{1.0f};
        bool mKeepAR{};
    };
}