//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Logger.hpp"
namespace
{
    std::unordered_map<std::string, oe::Logger*> loggers;
}

namespace oe
{
	Logger::Logger() = default;

	Logger::Logger(const std::string& file) { mFile.open(file, std::ios::out | std::ios::trunc); }

	void Logger::PrintMessage(const std::string& msg) const { Print("[OE::MESSAGE]", msg.c_str()); }

	void Logger::PrintWarning(const std::string& msg) const { Print("[OE::WARNING]", msg.c_str()); }

	void Logger::PrintError(const std::string& msg) const { Print("[OE::ERROR]", msg.c_str()); }

    Logger::~Logger()
    {
        for (const auto& [fst, snd] : loggers)
            if (snd == this) snd->mFile.close();
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