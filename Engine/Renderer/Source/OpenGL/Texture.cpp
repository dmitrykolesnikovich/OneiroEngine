//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Oneiro/Renderer/OpenGL/Texture.hpp"

#include "OpenGL/gl_core_4_5.hpp"
#include "stb/stb_image.h"
#define ONEIRO_IMPORT 1
//#include "Debugger/Debugger.hpp"

bool oe::Texture::PreLoad(const std::string& path)
{
    mData.data = stbi_load(path.c_str(), &mData.width, &mData.height, &mData.nrChannels, 0);
    if (mData.data) {
        return true;
    } else {
//        Debugger::GetLogger()->PrintWarning(dzl::string("Failed to load texture from ") + path + " path!");
        return false;
    }
}

bool oe::Texture::Load(const std::string& path)
{
    if (PreLoad(path)) {
        GenerateTexture();
        return true;
    } else {
        return false;
    }
}

void oe::Texture::Load()
{
    GenerateTexture();
}

void oe::Texture::Bind() const
{
    gl::ActiveTexture(gl::TEXTURE0);
    gl::BindTexture(gl::TEXTURE_2D, mID);
}

void oe::Texture::Bind(uint8_t id)
{
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(gl::TEXTURE_2D, 0);
}

void oe::Texture::UnBind() const
{
    gl::ActiveTexture(gl::TEXTURE0);
    gl::BindTexture(gl::TEXTURE_2D, 0);
}

void oe::Texture::UnLoad()
{
    gl::DeleteTextures(1, &mID);
}

void oe::Texture::GenerateTexture()
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

oe::Texture::~Texture()
{
    stbi_image_free(mData.data);
    UnLoad();
}
