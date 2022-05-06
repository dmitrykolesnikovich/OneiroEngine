//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Texture.hpp"

#include "OpenGL/gl_core_4_5.hpp"
#include "stb/stb_image.h"
#include "Oneiro/Core/Logger.hpp"

namespace oe::Renderer
{
    bool Texture::PreLoad(const std::string& path)
    {
        mData.Data = stbi_load(path.c_str(), &mData.Width, &mData.Height, &mData.NrChannels, 0);
        if (mData.Data)
        {
            mData.AR = static_cast<float>(mData.Width) / static_cast<float>(mData.Height);
            mIsLoaded = true;
            return true;
        }
        log::get("log")->warn(std::string("Failed to load texture from ") + path + " path!");
        stbi_image_free(mData.Data);
        return false;
    }

    void Texture::Load()
    {
        GenerateTexture();
        mIsLoaded = true;
    }

    void Texture::Bind() const
    {
        gl::ActiveTexture(gl::TEXTURE0);
        gl::BindTexture(gl::TEXTURE_2D, mID);
    }

    void Texture::Bind(uint8_t id) const
    {
        gl::ActiveTexture(gl::TEXTURE0 + id);
        gl::BindTexture(gl::TEXTURE_2D, mID);
    }

    void Texture::UnBind() const
    {
        gl::ActiveTexture(gl::TEXTURE0);
        gl::BindTexture(gl::TEXTURE_2D, 0);
    }

    bool Texture::IsLoaded() const { return mIsLoaded; }

    int Texture::GetWidth() const
    {
        return mData.Width;
    }

    int Texture::GetHeight() const
    {
        return mData.Height;
    }

    float Texture::GetAR() const
    {
        return mData.AR;
    }

    void Texture::UnLoad()
    {
        mIsLoaded = false;
        gl::DeleteTextures(1, &mID);
    }

    void Texture::GenerateTexture()
    {
        gl::GenTextures(1, &mID);
        Bind();
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST_MIPMAP_LINEAR);
        gl::TexImage2D(gl::TEXTURE_2D, 0, mData.NrChannels == 4 ? gl::SRGB_ALPHA : gl::SRGB, mData.Width, mData.Height, 0, mData.NrChannels == 4 ? gl::RGBA : gl::RGB, gl::UNSIGNED_BYTE, mData.Data);
        gl::GenerateMipmap(gl::TEXTURE_2D);
    }

    Texture::~Texture()
    {
        stbi_image_free(mData.Data);
        UnLoad();
    }
}