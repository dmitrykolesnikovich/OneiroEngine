//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>

#include "sol/state.hpp"

namespace oe::Lua
{
    class File
    {
    public:
        template<class... Args>
        void OpenLibraries(Args&& ... args)
        {
            mState.open_libraries(std::forward<Args>(args)...);
        }

        sol::protected_function_result LoadFile(const std::string& pathToFile, bool out = true)
        {
            if (out)
                mFile.open(pathToFile, std::ios::out);
            OpenLibraries(sol::lib::base);
            return mState.script_file(pathToFile);
        }

        std::fstream& GetFile() { return mFile; }
        sol::state* GetState() { return &mState; }
    private:
        sol::state mState{};
        std::fstream mFile{};
    };
}