#pragma once

#include "Base.hpp"
#include "Oneiro/Core/Root.hpp"

namespace oe::Renderer::Vulkan
{
    class UniformBuffer
    {
    public:
        void Create(VkDeviceSize size);

        template<class T>
        void PushData(const T& data) const;

        VkBuffer Get() const;

        void Destroy();
    private:
        VkBuffer mBuffer{};
        VkDeviceMemory mBufferMemory{};
    };

    template <class T>
    void UniformBuffer::PushData(const T& data) const
    {
        void* memData;
        auto device = Core::Root::Vulkan::GetLogicalDevice()->Get();
        vkMapMemory(device, mBufferMemory, 0, sizeof(T), 0, &memData);
        memcpy(memData, &data, sizeof(T));
        vkUnmapMemory(device, mBufferMemory);
    }
}
