//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>

#include "Oneiro/Renderer/OpenGL/Base.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Oneiro/Animation/Animation.hpp"

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
        const Texture* GetTexture() const { return mTexture; }
        float GetAlpha() const { return mAlpha; }
        void SetAlpha(float alpha) { mAlpha = alpha; }
        void SetUsingTextureAlpha(bool useTextureAlpha) { mUseTextureAlpha = useTextureAlpha; }
        bool IsLoaded() { return mTexture; }
    private:
        glm::mat4 mModel{1.0f};
        Texture* mTexture{};
        Shader mShader{};
        VertexBuffer mVBO;
        VertexArray mVAO;
        float mAlpha{1.0f};
        bool mKeepAR{};
        bool mUseTextureAlpha{};
    };
}