//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_TEXTURE_HPP
#define ONEIRO_TEXTURE_HPP

#include "DZL/types.hpp"
#include "DZL/string.hpp"
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    namespace Renderer
    {
        class OE_API Texture
        {
        public:
            ~Texture();
            bool PreLoad(const dzl::string& path);
            bool Load(const dzl::string& path);

            void Load();

            void UnLoad();
            void Bind() const;
            void Bind(dzl::uint8_t id);

            void UnBind() const;
        private:
            void GenerateTexture();
            struct TextureData
            {
                int width{};
                int height{};
                int nrChannels{};
                dzl::uint8_t* data{};
            };
            TextureData mData;
            dzl::uint32_t mID;
        };
    }
}

#endif //ONEIRO_TEXTURE_HPP
