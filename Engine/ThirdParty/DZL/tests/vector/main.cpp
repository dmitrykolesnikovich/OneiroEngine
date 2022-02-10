//
// Created by Dezlow on 29.01.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#include <iostream>

#if 1
#include "DZL/vector.hpp"
int main()
{
    dzl::vector<int> v1{1,2,3,4,5};
    dzl::vector<int> v2{1,2,3,4,5};

    std::cout << "FIRST VECTOR\n";
    for (auto val : v1)
        std::cout << val << '\n';

    std::cout << "\nSECOND VECTOR\n";

    for (auto val : v2)
        std::cout << val << '\n';

    std::cout << "\nEQUAL\n";

    std::cout << "v1 == v2 | " << (v1 == v2) << '\n';
    std::cout << "v1 != v2 | " << (v1 != v2) << '\n';
}
#else
#include <vector>
int main()
{
    std::vector<int> vec;b
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (auto val : vec)
        std::cout << val  << '\n';
}
#endif