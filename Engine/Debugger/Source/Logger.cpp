//
// Created by Dezlow on 09.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include <iostream>
#include "Debugger/Logger.hpp"

void oe::Debugger::Logger::PrintWarning(const dzl::string& str) const
{
    std::cout << "[OE::WARNING] " << str << "\n";
    mFile << "[OE::WARNING] " << str << "\n";
}

void oe::Debugger::Logger::PrintError(const dzl::string& str) const
{
    std::cerr << "[OE::ERROR] " << str << "\n";
    mFile << "[OE::ERROR] " << str << "\n";
}

void oe::Debugger::Logger::Init(const dzl::string& file)
{
    mFile.open(file);
    if (!mFile.is_open())
        std::cerr << "Failed to open Logger file!\n";
}

oe::Debugger::Logger::~Logger()
{
    mFile.close();
}
void oe::Debugger::Logger::PrintMessage(const dzl::string& str) const
{
    std::cout << "[OE::MESSAGE] " << str << "\n";
    mFile << "[OE::MESSAGE] " << str << "\n";
}