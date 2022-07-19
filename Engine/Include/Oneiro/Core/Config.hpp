//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Lua/LuaFile.hpp"
#include <string>

namespace oe::Core
{
    class Config
    {
      public:
        Config(const std::string& file);
        template <class T> T GetValue(const std::string& cfg);
        void WriteData(const std::string& cfg, const std::string& value);

        static Config* Get(const std::string& name);
        static Config* Add(const std::string& name);

      private:
        Lua::File mFileState;
    };

    template <class T> T Config::GetValue(const std::string& cfg)
    {
        return mFileState.GetState()->get_or<T>(cfg, {});
    }
} // namespace oe::Core
