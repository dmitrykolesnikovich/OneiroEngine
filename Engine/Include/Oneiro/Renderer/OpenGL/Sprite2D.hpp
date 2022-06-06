//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>

#include "glm/gtc/matrix_transform.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace oe::Renderer::GL
{
    class Sprite2D
    {
    public:
        void Init(const std::string& path, bool keepAspectRatio = true);
        bool Load();
        bool UnLoad();
        void Draw();
        void Move(const glm::vec3& pos);
        void Scale(const glm::vec3& scale);
        const Texture<gl::TEXTURE_2D>* GetTexture() const;
        float GetAlpha() const;
        void SetAlpha(float alpha);
        void SetUsingTextureAlpha(bool useTextureAlpha);
    private:
        TextureData* mTextureData{};
        glm::mat4 mModel{1.0f};
        Texture<gl::TEXTURE_2D>* mTexture{};
        Shader mShader{};
        Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW> mVBO{};
        VertexArray mVAO;
        float mAlpha{1.0f};
        bool mKeepAR{};
        bool mUseTextureAlpha{};
    };
}