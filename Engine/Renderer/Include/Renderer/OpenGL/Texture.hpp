//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include "DZL/types.hpp"
#include "DZL/string.hpp"

namespace oe
{
    namespace Renderer
    {
        class Texture
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

#endif //ENGINE_TEXTURE_HPP
