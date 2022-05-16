//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Scene/Scene.hpp"
#include "Oneiro/Scene/Entity.hpp"

namespace oe::Scene
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = {mRegistry.create(), this};
        entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        mRegistry.destroy(entity);
    }

    void Scene::DestroyAllEntities()
    {
        mRegistry.clear();
    }

    Entity Scene::GetEntity(const std::string& name)
    {
        for (auto [fst, snd]: mRegistry.storage())
        {
            for (const auto f: snd)
            {
                auto& comp = mRegistry.get<TagComponent>(f);
                if (comp.Tag == name)
                    return {to_entity(mRegistry, comp), this};
            }
        }
        return CreateEntity(name);
    }
}
