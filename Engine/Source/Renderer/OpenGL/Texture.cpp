//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Texture.hpp"

#include "OpenGL/gl_core_4_5.hpp"
#include "stb/stb_image.h"
#include "Oneiro/Core/Logger.hpp"

bool oe::Renderer::Texture::PreLoad()
{
    mData.data = stbi_load(mPath.c_str(), &mData.width, &mData.height, &mData.nrChannels, 0);
    if (mData.data) {
        mData.ar = static_cast<float>(mData.width) / static_cast<float>(mData.height);
        mIsLoaded = true;
        return true;
    }
    log::get("log")->warn(std::string("Failed to load texture from ") + mPath + " path!");
    stbi_image_free(mData.data);
    return false;
}

void oe::Renderer::Texture::Load()
{
    GenerateTexture();
    mIsLoaded = true;
}

void oe::Renderer::Texture::Bind() const
{
    gl::ActiveTexture(gl::TEXTURE0);
    gl::BindTexture(gl::TEXTURE_2D, mID);
}

void oe::Renderer::Texture::Bind(uint8_t id)
{
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(gl::TEXTURE_2D, mID);
}

void oe::Renderer::Texture::UnBind() const
{
    gl::ActiveTexture(gl::TEXTURE0);
    gl::BindTexture(gl::TEXTURE_2D, 0);
}

const oe::Renderer::Texture::Data& oe::Renderer::Texture::GetData() const { return mData; }

bool oe::Renderer::Texture::IsLoaded() const { return mIsLoaded; }

void oe::Renderer::Texture::UnLoad()
{
    mIsLoaded = false;
    gl::DeleteTextures(1, &mID);
}

void oe::Renderer::Texture::GenerateTexture()
{
    gl::GenTextures(1, &mID);
    Bind();
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST);
    gl::TexImage2D(gl::TEXTURE_2D, 0, mData.nrChannels == 4 ? gl::RGBA : gl::RGB,
                   mData.width, mData.height, 0, mData.nrChannels == 4 ? gl::RGBA : gl::RGB,
                   gl::UNSIGNED_BYTE, mData.data);
    gl::GenerateMipmap(gl::TEXTURE_2D);
}

oe::Renderer::Texture::~Texture()
{
    stbi_image_free(mData.data);
    UnLoad();
}
