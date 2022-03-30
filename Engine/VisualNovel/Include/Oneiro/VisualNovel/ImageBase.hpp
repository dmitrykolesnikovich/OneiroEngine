//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Renderer/OpenGL/Base.hpp"

#include <iostream>

namespace oe::VisualNovel
{
    class ImageBase
    {
    public:
        virtual void Init() = 0;
        virtual void Load(const std::string& path) { LoadTexture(path); };
        virtual void Load() { LoadTexture(); };
        virtual void UnLoad() { UnLoadTexture(); };
        virtual void Draw() { DrawTexture(); };
    protected:
        void InitBuffers(uint32_t verticesSize, const float* vertices)
        {
            mVAO.Init();
            mVAO.Bind();
            mVBO.Create(verticesSize, vertices);
            Renderer::VertexBuffer::PushLayout(0,3,3,0);
            mVAO.UnBind();
            mVBO.UnBind();
        }

        bool LoadTexture()
        {
            if (!mIsTextureLoaded)
            {
                mTexture.Load();
                mIsTextureLoaded = true;
            }
            return mIsTextureLoaded;
        }

        bool LoadTexture(const std::string& path)
        {
            if (!mIsTextureLoaded)
            {
                mTexture.Load(path);
                mIsTextureLoaded = true;
            }
            return mIsTextureLoaded;
        }

        bool UnLoadTexture()
        {
            if (mIsTextureLoaded)
            {
                mTexture.UnLoad();
                mIsTextureLoaded = false;
            }
            return mIsTextureLoaded;
        }

        void DrawTexture()
        {
            mShader.Use();
            mVAO.Bind();
            mTexture.Bind();
            gl::DrawArrays(gl::TRIANGLES, 0, 6);
            mTexture.UnBind();
            mVAO.UnBind();
        }

        void LoadShaderFromSource(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
        {
            mShader.LoadFromSource(vertexShaderSrc, fragmentShaderSrc);
        }

        const Renderer::Shader* GetShader() { return &mShader; }
        const Renderer::Texture* GetTexture() { return &mTexture; }
    private:
        Renderer::Texture mTexture{};
        Renderer::Shader mShader{};
        Renderer::VertexBuffer mVBO;
        Renderer::VertexArray mVAO;
        bool mIsTextureLoaded{};
    };
}