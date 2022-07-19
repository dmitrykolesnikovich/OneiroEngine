//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cassert>
#include <stdexcept>
#include <string>

#define OE_THROW_ERROR(who, error)                                                                                                         \
    {                                                                                                                                      \
        throw std::runtime_error(std::string("[") + (who) + "] " +                                                                         \
                                 ((error) + std::string(" ") + std::string(__FILE__) + ":" + std::to_string(__LINE__)));                   \
    }
#define VK_CHECK_RESULT(val, error)                                                                                                        \
    {                                                                                                                                      \
        VkResult res = (val);                                                                                                              \
        if (res != VK_SUCCESS)                                                                                                             \
        {                                                                                                                                  \
            OE_THROW_ERROR("Renderer::Vulkan", error)                                                                                      \
            assert(res == VK_SUCCESS);                                                                                                     \
        }                                                                                                                                  \
    }
