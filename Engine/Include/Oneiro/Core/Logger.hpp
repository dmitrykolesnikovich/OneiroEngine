//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>
#include <iostream>

#include <unordered_map>

namespace oe
{
    class Logger
    {
    public:
        Logger();
        Logger(const std::string& file);
        ~Logger();

        static bool Create(const std::string& loggerName, const std::string& fileName);
        void PrintMessage(const std::string& msg) const;
        void PrintWarning(const std::string& msg) const;
        void PrintError(const std::string& msg) const;
        static const Logger* Get(const char* name);
    private:
        void Print(const char* type, const char* msg) const;
        mutable std::fstream mFile;
    };
}