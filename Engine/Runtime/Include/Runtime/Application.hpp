//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#ifndef ENGINE_APPLICATION_HPP
#define ENGINE_APPLICATION_HPP

#include "Core/Window.hpp"

namespace oe
{
    namespace runtime
    {
        class Application
        {
        public:
            void Init();
            void Run();
        private:
            Core::Window mWindow;
        };
    }
}

#endif //ENGINE_APPLICATION_HPP
