//
// Created by Dezlow on 14.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#ifndef ENGINE_API_APPLICATION_HPP
#define ENGINE_API_APPLICATION_HPP

namespace oe
{
    namespace API
    {
        class Application
        {
        public:
            Application() = default;
            ~Application() = default;

            virtual bool Init() = 0;
            virtual bool Update() = 0;
            virtual void Close() = 0;
        };
    }
}

#endif //ENGINE_API_APPLICATION_HPP
