//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#ifndef ENGINE_DEBUGGER_HPP
#define ENGINE_DEBUGGER_HPP

#include "Debugger/Logger.hpp"

namespace oe
{
    namespace Debugger
    {
        void Init();
        void Shutdown();
        const Logger* GetLogger();
    }
}

#endif //ENGINE_DEBUGGER_HPP
