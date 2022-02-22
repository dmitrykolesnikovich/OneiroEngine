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

namespace oe
{
    namespace Core
    {
        class Logger
        {
        public:
            Logger() = default;
            ~Logger()
            {
                mFile->close();
            }

            static bool Create(const dzl::string& fileName)
            {
                delete mFile;
                mFile = new std::ofstream;
                mFile->open(fileName);
                if (!mFile->is_open())
                {
                    std::cerr << "Failed to open logger file!\n";
                    return false;
                }
                return true;
            }

            static void PrintMessage(const dzl::string& str)
            {
                std::cout << "[OE::MESSAGE] " << str << "\n";
                *mFile << "[OE::MESSAGE] " << str << "\n";
            }

            static void PrintWarning(const dzl::string& str)
            {
                std::cout << "[OE::WARNING] " << str << "\n";
                *mFile << "[OE::WARNING] " << str << "\n";
            }

            static void PrintError(const dzl::string& str)
            {
                std::cerr << "[OE::ERROR] " << str << "\n";
                *mFile << "[OE::ERROR] " << str << "\n";
            }

        private:
            static std::ofstream* mFile;
        };
    }
}

std::ofstream* oe::Core::Logger::mFile{};
#endif //ONEIRO_CORE_LOGGER_HPP
