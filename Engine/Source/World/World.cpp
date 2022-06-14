//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/World/World.hpp"
#include "Oneiro/World/Entity.hpp"

namespace oe::World
{
    World::World()
    {
    }

    World::~World()
    {
    }

    Entity World::CreateEntity(const std::string& name)
    {
        Entity entity = {mRegistry.create(), this};
        entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void World::DestroyEntity(Entity entity)
    {
        mRegistry.destroy(entity);
    }

    void World::DestroyAllEntities()
    {
        mRegistry.clear();
    }

    Entity World::GetEntity(const std::string& name)
    {
        for (auto [fst, snd] : mRegistry.storage())
        {
            for (const auto f : snd)
            {
                auto& comp = mRegistry.get<TagComponent>(f);
                if (comp.Tag == name)
                    return {to_entity(mRegistry, comp), this};
            }
        }
        return CreateEntity(name);
    }
}
