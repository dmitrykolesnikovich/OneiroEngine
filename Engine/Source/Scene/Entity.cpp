//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Scene/Entity.hpp"

namespace oe::Scene
{
    Entity::Entity() = default;

    Entity::Entity(entt::entity handle, Scene* scene) : mHandle(handle), mScene(scene)
    {
    }
}
