//
// Created by Dezlow on 22.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_CORE_LOGGER_HPP
#define ONEIRO_CORE_LOGGER_HPP

#include "DZL/string.hpp"
#include <fstream>
#include <iostream>

#include <unordered_map>

#define OE_DLL_EXPORT
#include "Oneiro.hpp"

namespace oe { class Logger; }
static std::unordered_map<const char*, oe::Logger*> loggers;

namespace oe
{
    class Logger
    {
    public:
        Logger() = default;
        Logger(const char* file) { mFile.open(file, std::ios::out | std::ios::trunc); }
        ~Logger()
        {
            for (const auto& log : loggers)
            {
                log.second->mFile.close();
            }
        }

        static bool Create(const char* loggerName, const dzl::string& fileName)
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

        void PrintMessage(const dzl::string& msg) const
        {
            Print("[OE::MESSAGE]", msg);
        }

        void PrintWarning(const dzl::string& msg) const
        {
            Print("[OE::WARNING]", msg);
        }

        void PrintError(const dzl::string& msg) const
        {
            Print("[OE::ERROR]", msg);
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



#endif //ONEIRO_CORE_LOGGER_HPP
