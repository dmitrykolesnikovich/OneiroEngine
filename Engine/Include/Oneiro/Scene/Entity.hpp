//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Scene.hpp"
#include "Components.hpp"

#include "Oneiro/Core/Oneiro.hpp"

namespace oe::Scene
{
    class Entity
    {
    public:
        Entity();
        Entity(entt::entity handle, Scene* scene);

        template<typename T, typename... Args>
        T& AddComponent(Args&& ... args)
        {
            if (HasComponent<T>())
                return mScene->mRegistry.get<T>(mHandle);
            T& component = mScene->mRegistry.emplace<T>(mHandle, std::forward<Args>(args)...);
            return component;
        }

        template<class T>
        T& GetComponent() const
        {
            if (!HasComponent<T>()) OE_THROW_ERROR("Scene",
                                                   "Entity \"" + GetComponent<TagComponent>().Tag +
                                                           "\" does not have component!");
            return mScene->mRegistry.get<T>(mHandle);
        }

        template<typename T>
        [[nodiscard]] bool HasComponent() const
        {
            return mScene->mRegistry.try_get<T>(mHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            mScene->mRegistry.remove<T>(mHandle);
        }

        operator bool() const
        {
            return mHandle != entt::null;
        }

        operator entt::entity() const
        {
            return mHandle;
        }

        operator uint32_t() const
        {
            return static_cast<uint32_t>(mHandle);
        }

        bool operator==(const Entity& other) const
        {
            return mHandle == other.mHandle && mScene == other.mScene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

    private:
        entt::entity mHandle{};
        Scene* mScene{};
    };
}
