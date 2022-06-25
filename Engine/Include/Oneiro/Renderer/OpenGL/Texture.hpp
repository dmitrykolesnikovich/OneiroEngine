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
	struct TextureData
	{
		int Width{}, Height{}, Channels{};
		uint8_t* Data{};
		std::string Path;
	};

	template <int TextureType, bool Mipmaps = true>
	class Texture
	{
	public:
		Texture() = default;
		Texture(const Texture&) = delete;
		Texture(const std::string& path) { mData.Path = path; }

		~Texture()
		{
			if (mID)
				gl::DeleteTextures(1, &mID);
		}

		void Generate()
		{
			gl::GenTextures(1, &mID);
		}

        static void TexImage2D(int internalFormat, int width, int height, int border, int format, int type, const void* data)
		{
			gl::TexImage2D(TextureType, 0, internalFormat, width, height, 0, format, type, data);
		}

		TextureData* GetData() { return &mData; }

        [[nodiscard]] const TextureData* GetData() const { return &mData; }

        static void TexImage2D(int textureType, int internalFormat, int width, int height, int border, int format, int type,
                               const void* data)
		{
			gl::TexImage2D(textureType, 0, internalFormat, width, height, 0, format, type, data);
		}

        static void TexParameter(int name, int param)
		{
			gl::TexParameteri(TextureType, name, param);
		}

        static void GenerateMipmap()
		{
			gl::GenerateMipmap(TextureType);
		}

		constexpr void Bind(uint8_t id = 0) const
		{
			gl::ActiveTexture(gl::TEXTURE0 + id);
			gl::BindTexture(gl::TEXTURE_2D, mID);
		}

        static constexpr void UnBind()
        {
			gl::BindTexture(TextureType, 0);
		}

        [[nodiscard]] uint32_t Get() const { return mID; }
	private:
		TextureData mData{};
		uint32_t mID{};
	};
	
	template <int TextureType, bool Mipmaps = true>
	bool PreLoad2DTexture(Texture<TextureType, Mipmaps>& texture)
	{
		auto data = texture.GetData();
		data->Data = stbi_load(data->Path.c_str(), &data->Width, &data->Height, &data->Channels, 0);
		if (data->Data)
			return true;
		return false;
	}

	// TODO: Rewrite with using templates
	bool Load2DTexture(const char* path, Texture<gl::TEXTURE_2D>* texture, TextureData* textureData = nullptr);

	bool Load2DTexture(const std::string& path, Texture<gl::TEXTURE_2D>* texture, TextureData* textureData = nullptr);

	bool Load2DTexture(Texture<gl::TEXTURE_2D>* texture);
}
