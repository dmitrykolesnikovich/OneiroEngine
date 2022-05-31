//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cstdint>
#include "OpenGL/gl_core_4_5.hpp"

#define IS_SAME(auto) \
requires (std::is_same<T, auto>::value)

#define IS_SAME_TEMPLATE(auto) \
template<class T> \
IS_SAME(auto)
