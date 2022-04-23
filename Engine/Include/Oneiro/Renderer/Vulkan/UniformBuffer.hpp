//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "DescriptorPool.hpp"
#include "DescriptorSet.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::Vulkan
{
    class UniformBuffer
    {
    public:
        void Create(int binding, VkShaderStageFlagBits stage, VkDeviceSize size);

        template<class T>
        void PushData(const T& data) const;

        [[nodiscard]] VkBuffer Get() const;

        void Destroy();
    private:
        DescriptorPool mDescriptorPool;
        DescriptorSet mDescriptorSet;
        VkBuffer mBuffer{};
        VkDeviceMemory mBufferMemory{};
    };

    template <class T>
    void UniformBuffer::PushData(const T& data) const
    {
        void* memData;
        const auto device = GetLogicalDevice()->Get();
        vkMapMemory(device, mBufferMemory, 0, sizeof(T), 0, &memData);
        memcpy(memData, &data, sizeof(T));
        vkUnmapMemory(device, mBufferMemory);
        mDescriptorSet.Bind();
    }
}
