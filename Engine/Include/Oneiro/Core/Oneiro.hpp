//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cassert>
#include <string>
#include <stdexcept>
#include <source_location>

#define OE_THROW_ERROR(who, error) \
{ \
    auto location = std::source_location::current(); \
    throw std::runtime_error(std::string("[") + (who) \
                             + "] " + location.file_name() + "(" + \
                             std::to_string(location.line()) + ":" + std::to_string(location.column()) + \
                             ") '" + location.function_name() + "': " + ((error) + std::string(" "))); \
}

#define VK_CHECK_RESULT(val, error) \
{ \
    VkResult res = (val); \
    if (res != VK_SUCCESS) \
    { \
        OE_THROW_ERROR("Renderer::Vulkan", error) \
        assert(res == VK_SUCCESS); \
    }  \
}
