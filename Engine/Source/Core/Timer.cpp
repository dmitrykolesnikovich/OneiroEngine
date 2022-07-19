//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Timer.hpp"

namespace oe::Core
{
    Timer::Timer() = default;

    void Timer::Start()
    {
        mStart = std::chrono::high_resolution_clock::now();
    }

    float Timer::End() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart).count() * 0.001f *
               0.001f * 0.001f * 1000.0f;
    }
} // namespace oe::Core
