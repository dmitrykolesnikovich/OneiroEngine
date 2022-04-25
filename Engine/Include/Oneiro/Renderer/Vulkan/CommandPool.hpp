//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "../Renderer.hpp"

namespace oe::Renderer::Vulkan
{
	class CommandPool
	{
	public:
		void Create();

		void Destroy();

		[[nodiscard]] VkCommandPool Get() const;
		[[nodiscard]] const VkCommandPool* GetPtr() const;
	private:
		VkCommandPool mCommandPool{};
	};
}
