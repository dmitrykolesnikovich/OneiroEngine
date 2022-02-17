//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#ifndef ENGINE_LOGGER_HPP
#define ENGINE_LOGGER_HPP

#include <iostream>
#include <fstream>
#include "DZL/string.hpp"

namespace oe
{
    namespace Debugger
    {
        class Logger
        {
        public:
            Logger() = default;
            ~Logger();
            void Init(const dzl::string& file);
            void PrintWarning(const dzl::string& str) const;
            void PrintError(const dzl::string& str) const;
            void PrintMessage(const dzl::string& str) const;
        private:
            mutable std::ofstream mFile;
        };
    }
}

#endif //ENGINE_LOGGER_HPP
