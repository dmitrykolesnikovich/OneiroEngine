//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Texture.hpp"

namespace oe::Renderer::GL
{
	bool GL::Load2DTexture(const char* path, Texture<gl::TEXTURE_2D>* texture,
	                       TextureData* textureData)
	{
		uint8_t* data{};
		int width{}, height{}, nrChannels{};
		if (textureData)
		{
			data = stbi_load(path, &textureData->Width, &textureData->Height,
			                 &textureData->Channels, 0);
			width = textureData->Width;
			height = textureData->Height;
			nrChannels = textureData->Channels;
		}
		else
		{
			data = stbi_load(path, &width, &height, &nrChannels, 0);
		}
		if (data)
		{
			int internalFormat{};
			int format{};
			switch (nrChannels)
			{
			case 4:
				internalFormat = gl::SRGB_ALPHA;
				format = gl::RGBA;
				break;
			case 3:
				internalFormat = gl::SRGB;
				format = gl::RGB;
				break;
			default:
				internalFormat = gl::RED;
				format = gl::RED;
				break;
			}

			texture->Generate();
			texture->Bind();
			texture->TexImage2D(internalFormat, width, height, 0, format, gl::UNSIGNED_BYTE, data);
			texture->TexParameter(gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
			texture->TexParameter(gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
			texture->TexParameter(gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
			texture->TexParameter(gl::TEXTURE_MAG_FILTER, gl::NEAREST_MIPMAP_LINEAR);
			texture->GenerateMipmap();
			stbi_image_free(data);
			return true;
		}
		stbi_image_free(data);
		return false;
	}

	bool Load2DTexture(const std::string& path, Texture<gl::TEXTURE_2D>* texture,
	                   TextureData* textureData)
	{
		return Load2DTexture(path.c_str(), texture, textureData);
	}

	bool GL::Load2DTexture(Texture<gl::TEXTURE_2D>* texture)
	{
		int internalFormat{};
		int format{};
		const auto* data = texture->GetData();
		switch (data->Channels)
		{
		case 4:
			internalFormat = gl::SRGB_ALPHA;
			format = gl::RGBA;
			break;
		case 3:
			internalFormat = gl::SRGB;
			format = gl::RGB;
			break;
		default:
			internalFormat = gl::RED;
			format = gl::RED;
			break;
		}

		texture->Generate();
		texture->Bind();
		texture->TexImage2D(internalFormat, data->Width, data->Height, 0, format, gl::UNSIGNED_BYTE,
		                    data->Data);
		texture->TexParameter(gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
		texture->TexParameter(gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
		texture->TexParameter(gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
		texture->TexParameter(gl::TEXTURE_MAG_FILTER, gl::NEAREST_MIPMAP_LINEAR);
		texture->GenerateMipmap();
		texture->UnBind();
		return true;
	}
}
