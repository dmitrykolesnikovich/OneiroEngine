//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_TEXTURE_HPP
#define ONEIRO_TEXTURE_HPP

#define OE_DLL_EXPORT
#include <string>

#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    class OE_API Texture
    {
    public:
        ~Texture();
        bool PreLoad(const std::string& path);
        bool Load(const std::string& path);

        void Load();

        void UnLoad();
        void Bind() const;
        void Bind(uint8_t id);

        void UnBind() const;
    private:
        void GenerateTexture();
        struct TextureData
        {
            int width{};
            int height{};
            int nrChannels{};
            uint8_t* data{};
        };
        TextureData mData;
    	uint32_t mID;
    };
}

#endif //ONEIRO_TEXTURE_HPP
