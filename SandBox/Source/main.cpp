//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Runtime/EntryPoint.hpp"

std::shared_ptr<oe::Runtime::Application> oe::Runtime::CreateApplication()
{
    return std::make_shared<Application>();
}