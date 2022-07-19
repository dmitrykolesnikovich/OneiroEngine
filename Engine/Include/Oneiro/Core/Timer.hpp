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
        Timer();
        void Start();

        /**
         * @return Returns total time in milliseconds
         */
        [[nodiscard]] float End() const;

      private:
        std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    };
} // namespace oe::Core
