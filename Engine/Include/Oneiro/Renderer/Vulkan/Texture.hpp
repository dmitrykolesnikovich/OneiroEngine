//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"
#include <string>

namespace oe::Renderer::Vulkan
{
    // TODO: Move to renderer utils
    struct Image
    {
        static void TransitionLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipmaps = 1);

        static void Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkSampleCountFlagBits numSamples, uint32_t mipmaps = 1);

        static void CreateView(VkImageView* imageView, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipmaps = 1);

    private:
        static void CreateViewInfo(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipmaps, VkImageViewCreateInfo& viewInfo);
    };

    class Texture
    {
    private:
        struct Data;
    public:
        /**
         * \brief Loading texture from path. \n
         * WARNING: Load texture before creating uniform buffer.
         * \param path path to texture
         */
        void Load(const std::string& path);
        void Load();
        void UnLoad();

        void Destroy();

        [[nodiscard]] VkImage GetImage() const;
        [[nodiscard]] VkImageView GetView() const;
        [[nodiscard]] VkSampler GetSampler() const;
        bool IsLoaded() const;

        const Data& GetData() const
        {
            return mData;
        }

    private:
        static void CreateTextureSampler(VkSampler* sampler, uint32_t mipmaps);

        struct Data
        {
            uint8_t* Pixels{};
            int Width{};
            int Height{};
            int Channels{};
            float Ar;
        };

        Data mData;

        VkImage mImage{};
        VkImageView mImageView{};
        VkSampler mSampler{};
        VkDeviceMemory mDeviceMemory{};

        bool mIsLoaded{};
    };
}
