//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>
#include <iostream>

#include <unordered_map>

#define OE_DLL_EXPORT
#include "Oneiro.hpp"

namespace oe { class Logger; }
static std::unordered_map<std::string, oe::Logger*> loggers;

namespace oe
{
    class Logger
    {
    public:
        Logger() = default;
        Logger(const std::string& file) { mFile.open(file, std::ios::out | std::ios::trunc); }
        ~Logger()
        {
            for (const auto& log : loggers)
            {
                log.second->mFile.close();
            }
        }

        static bool Create(const std::string& loggerName, const std::string& fileName)
        {
            static Logger log(fileName);
            loggers[loggerName] = &log;
            if (!log.mFile.is_open())
            {
                std::cerr << "Failed to open log file!\n";
                return false;
            }
            return true;
        }

        void PrintMessage(const std::string& msg) const
        {
            Print("[OE::MESSAGE]", msg.c_str());
        }

        void PrintWarning(const std::string& msg) const
        {
            Print("[OE::WARNING]", msg.c_str());
        }

        void PrintError(const std::string& msg) const
        {
            Print("[OE::ERROR]", msg.c_str());
        }

        static const Logger* Get(const char* name) { return loggers[name]; }
    private:
        void Print(const char* type, const char* msg) const
        {
            mFile << type << " " << msg << "\n";
            std::cerr << type << " " << msg << "\n";
        }
        mutable std::fstream mFile;
    };
}