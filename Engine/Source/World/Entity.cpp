//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/World/Entity.hpp"

namespace oe::World
{
    Entity::Entity() = default;

    Entity::Entity(entt::entity handle, World* scene) : mHandle(handle), mScene(scene) {}
} // namespace oe::World
