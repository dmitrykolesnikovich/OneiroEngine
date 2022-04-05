//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Logger.hpp"
namespace
{
    static std::unordered_map<std::string, oe::Logger*> loggers;
}

namespace oe
{
    Logger::~Logger()
    {
        for (const auto& log : loggers)
        {
            log.second->mFile.close();
        }
    }

    bool Logger::Create(const std::string& loggerName, const std::string& fileName)
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
    const Logger* Logger::Get(const char* name) { return loggers[name]; }

    void Logger::Print(const char* type, const char* msg) const
    {
        mFile << type << " " << msg << "\n";
        std::cerr << type << " " << msg << "\n";
    }
}