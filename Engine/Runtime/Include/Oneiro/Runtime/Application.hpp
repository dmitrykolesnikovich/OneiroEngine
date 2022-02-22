//
// Created by Dezlow on 22.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_RUNTIME_APPLICATION_HPP
#define ONEIRO_RUNTIME_APPLICATION_HPP

namespace oe
{
    namespace Runtime
    {
        class Application
        {
        public:
            virtual bool Init() = 0;
            virtual bool Update() = 0;
            virtual void Close() = 0;
        };
    }
}

#endif //ONEIRO_RUNTIME_APPLICATION_HPP
