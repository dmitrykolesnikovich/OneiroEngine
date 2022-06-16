//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "World.hpp"

namespace oe::World
{
	class WorldManager
	{
	public:
		WorldManager();

		World* GetWorld();
		void SaveWorld(const std::string& filePath, const std::string& worldName, bool reWrite = true);
		bool LoadWorld(const std::string& filePath) const;
	private:
		inline static WorldManager* mInstance{};
		std::unique_ptr<World> mWorld{};
	};
}
