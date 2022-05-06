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
        T* Add(const std::shared_ptr<T>& resourcePtr)
        {
            mResources.emplace_back(resourcePtr);
            return resourcePtr.get();
        }
        [[nodiscard]] T* Get(size_t index) const { return mResources[index].get(); }
        void Remove(const T& resource) { mResources.erase(resource); }
        void Remove(size_t id) { mResources.erase(id); }
        constexpr std::vector<std::shared_ptr<T>>& GetResources() { return mResources; }
    private:
        std::vector<std::shared_ptr<T>> mResources{};
    };
}
