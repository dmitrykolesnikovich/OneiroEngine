//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include "Oneiro/Runtime/Engine.hpp"
#include "Oneiro/Core/Logger.hpp"

class SandBoxApp : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::Logger::Get("log")->PrintMessage("Initializing...");
        return true;
    }
    bool Update() override { return true; }
    void Close() override
    {
        oe::Logger::Get("log")->PrintMessage("Closing...");
    }
};

int main()
{
    SandBoxApp app{};
    oe::Runtime::Engine::Run(app);
}