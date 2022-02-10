//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include <iostream>
#include "Runtime/Engine.hpp"

int main()
{
    try
    {
        oe::runtime::Engine::Run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}