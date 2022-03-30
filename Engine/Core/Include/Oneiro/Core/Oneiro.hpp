//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

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
