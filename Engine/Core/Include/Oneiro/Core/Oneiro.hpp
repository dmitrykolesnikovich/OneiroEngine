//
// Created by Dezlow on 22.02.2022.
// Copyright (c) 2022 Oneiro Games All rights reserved.
//


#pragma once

#ifndef ONEIRO_HPP
#define ONEIRO_HPP

#ifdef _WIN32
    #ifndef OE_DLL_IMPORT
        #ifdef OE_DLL_EXPORT
            #define OE_API __declspec(dllexport)
        #else
            #define OE_DLL_IMPORT
            #define OE_API __declspec(dllimport)
        #endif
    #endif
    #ifdef OE_DLL_IMPORT
        #define OE_API __declspec(dllimport)
#endif
#else
    #define OE_API
#endif

#endif //ONEIRO_HPP
