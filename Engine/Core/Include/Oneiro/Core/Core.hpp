//
// Created by Dezlow on 20.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_CORE_HPP
#define ONEIRO_CORE_HPP

#define DLL_EXPORT
#include "Oneiro.hpp"
#include "Logger.hpp"

namespace oe
{
    namespace Core
    {
        void Init() { Logger::Create("log.txt"); }
        void Shutdown() { }
    }
}

#endif //ONEIRO_CORE_HPP
