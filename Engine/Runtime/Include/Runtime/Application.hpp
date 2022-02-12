//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#ifndef ENGINE_APPLICATION_HPP
#define ENGINE_APPLICATION_HPP

#include <memory>
#include "Core/Window.hpp"

namespace oe
{
    namespace Runtime
    {
        class Application
        {
        public:
            void Run();
        private:
            Core::Window mWindow;
        };

        std::shared_ptr<Application> CreateApplication();
    }
}

#endif //ENGINE_APPLICATION_HPP
