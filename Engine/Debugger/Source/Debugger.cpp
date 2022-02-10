//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Debugger/Debugger.hpp"

namespace
{
    oe::Debugger::Logger log;
}

void oe::Debugger::Init()
{
    log.Init("log.txt");
}

void oe::Debugger::Shutdown()
{
    log.~Logger();
}

const oe::Debugger::Logger* oe::Debugger::GetLogger()
{
    return &log;
}