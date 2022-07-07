//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "ValidationLayers.hpp"

namespace oe::Renderer::Vulkan
{
    class Instance
    {
      public:
        void Create(bool enableValidationLayer = true);

        [[nodiscard]] VkInstance Get() const;
        [[nodiscard]] const VkInstance* GetPtr() const;

        void Destroy() const;

      private:
        VkInstance mVkInstance{};
        ValidationLayers mValidationLayers;
    };
} // namespace oe::Renderer::Vulkan
