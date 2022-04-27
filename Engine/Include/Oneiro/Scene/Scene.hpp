//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "entt/entt.hpp"

namespace oe::Scene
{
	class Entity;
	class SceneManager;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = {});
		void DestroyEntity(Entity entity);
		Entity GetEntity(const std::string& name = {});
	private:
		entt::registry mRegistry{};
		std::string mName{};
		friend class Entity;
		friend class SceneManager;
	};
}
