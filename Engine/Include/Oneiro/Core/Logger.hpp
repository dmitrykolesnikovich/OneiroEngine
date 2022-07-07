//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "spdlog/spdlog.h"

#define OE_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define OE_LOG_WARNING(...) SPDLOG_WARNING(__VA_ARGS__)
#define OE_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)

namespace oe::log
{
    using namespace spdlog;
}
