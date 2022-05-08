//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>

namespace oe::Renderer
{
    class Texture
    {
        struct TextureData;
    public:
        ~Texture();
        void Init(const std::string& path);
        bool PreLoad();
        void Load();
        void UnLoad();
        void Bind() const;
        void Bind(uint8_t id) const;
        void UnBind() const;
        bool IsLoaded() const;

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        float GetAR() const;
    private:
        struct TextureData
        {
            int Width{};
            int Height{};
            int NrChannels{};
            float AR{};
            uint8_t* Data{};
        };
        std::string mTexturePath;
        void GenerateTexture();
        TextureData mData;
        uint32_t mID{};
        bool mIsLoaded{};
    };
}