//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Scene.hpp"

namespace oe::Scene
{
	class SceneManager
	{
	public:
		SceneManager(Scene* scene) : mScene(scene) {}

		void Save(const std::string& filepath, const std::string& sceneName);

		bool Load(const std::string& filePath) const;
	private:
		Scene* mScene{};
	};
}
