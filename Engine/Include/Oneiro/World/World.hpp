//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "entt/entt.hpp"

namespace oe::World
{
	class Entity;
	class WorldManager;

	class World
	{
	public:
		World();
		~World();

		Entity CreateEntity(const std::string& name = {});
		void DestroyEntity(Entity entity);
		Entity GetEntity(const std::string& name = {});
		entt::registry& GetEntities() { return mRegistry; }
		void DestroyAllEntities();
	private:
		entt::registry mRegistry{};
		std::string mName{};
		friend class Entity;
		friend class WorldManager;
	};
}
