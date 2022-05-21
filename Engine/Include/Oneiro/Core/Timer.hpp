//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <chrono>

namespace oe::Core
{
    class Timer
    {
    public:
        Timer() { }
        void Start() { mStart = std::chrono::high_resolution_clock::now(); }

        /**
         * @return returns total time in milliseconds
         */
        [[nodiscard]] float End() const
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now() - mStart).count() * 0.001f * 0.001f *
                    0.001f * 1000.0f;
        }
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    };
}
