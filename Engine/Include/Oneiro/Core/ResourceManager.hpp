//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>
#include <memory>

namespace oe::Core
{
	template<class T>
	class ResourceManager
	{
	public:
		T* AddItem(const std::shared_ptr<T>& itemPtr) { mResources.emplace_back(itemPtr); return itemPtr.get(); }

		T* GetItem(size_t id) const { return mResources[id].get(); }

		void RemoveResource(const T& resource)
		{
			mResources.erase(resource);
		}

		void RemoveResource(size_t id)
		{
			mResources.erase(id);
		}

		constexpr std::vector<std::shared_ptr<T>>& GetResources() { return mResources; }
	private:
		std::vector<std::shared_ptr<T>> mResources{};
	};
}
