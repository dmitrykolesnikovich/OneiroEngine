//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>
#include "stb/stb_image.h"
#include "Base.hpp"

namespace oe::Renderer::GL
{
    template<int TextureType, bool Mipmaps = true>
    class Texture
    {
    public:
        ~Texture()
        {
            gl::DeleteTextures(1, &mID);
        }

        void Generate()
        {
            gl::GenTextures(1, &mID);
        }

        void TexImage2D(int internalFormat, int width, int height, int border, int format, int type,
                        const void* data)
        {
            gl::TexImage2D(TextureType, 0, internalFormat, width, height, 0, format, type, data);
        }

        void TexImage2D(int textureType, int internalFormat, int width, int height, int border,
                        int format, int type, const void* data)
        {
            gl::TexImage2D(textureType, 0, internalFormat, width, height, 0, format, type, data);
        }

        void TexParameter(int name, int param)
        {
            gl::TexParameteri(TextureType, name, param);
        }

        void GenerateMipmap()
        {
            gl::GenerateMipmap(TextureType);
        }

        constexpr void Bind(uint8_t id = 0) const
        {
            gl::ActiveTexture(gl::TEXTURE0 + id);
            gl::BindTexture(gl::TEXTURE_2D, mID);
        }

        constexpr void UnBind() const
        {
            gl::BindTexture(TextureType, 0);
        }

        uint32_t Get() const { return mID; }
    private:
        uint32_t mID{};
    };

    struct TextureData
    {
        int Width{}, Height{}, Channels{};
    };

    bool Load2DTexture(const char* path, Texture<gl::TEXTURE_2D>* texture,
                       TextureData* textureData = nullptr);

    bool Load2DTexture(const std::string& path, Texture<gl::TEXTURE_2D>* texture,
                       TextureData* textureData = nullptr);
}