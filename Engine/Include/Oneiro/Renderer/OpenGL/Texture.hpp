//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>

namespace oe::Renderer::GL
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
        [[nodiscard]] float GetAR() const;
        std::string GetPath() const { return mTexturePath; }

        static void GenerateTexture(uint32_t* textureID, uint32_t nrChannels, int width, int height,
                                    uint8_t* data, int clamp = 0x812D);
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
        TextureData mData;
        uint32_t mID{};
        bool mIsLoaded{};
    };
}