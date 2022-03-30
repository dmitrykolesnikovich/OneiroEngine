//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT

#include <string>

#include "Oneiro/Core/Oneiro.hpp"

namespace oe::Renderer
{
    class OE_API Texture
    {
        struct Data
        {
            int width{};
            int height{};
            int nrChannels{};
            float ar{};
            uint8_t* data{};
        };
    public:
        ~Texture();
        bool PreLoad(const std::string& path);
        bool Load(const std::string& path);
        void Load();
        void UnLoad();
        void Bind() const;
        void Bind(uint8_t id);
        void UnBind() const;
        [[nodiscard]] const Data& GetData() const { return mData; }
    private:
        void GenerateTexture();
        Data mData;
        uint32_t mID;
    };
}