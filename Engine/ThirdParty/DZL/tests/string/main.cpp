//
// Created by Dezlow on 29.01.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include <iostream>

#if 1
#include "DZL/string.hpp"

const char* func(const char* str)
{
    return str = "Bye!";
}

int main()
{
    dzl::string str("Hello");
    std::cout << str << '\n';
    std::cout << str + " World!" << '\n';
    str = dzl::string("Hello");
    dzl::string str2(str);
    dzl::string str3(" World!");
    std::cout << str2 + str3 << '\n';
    std::cout << (str2 == "Hello World!") << '\n';
    std::cout << func(str2) << '\n';
}
#else
#include <string>
int main()
{
    std::string str;
}
#endif