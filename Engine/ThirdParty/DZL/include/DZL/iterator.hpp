//
// Created by Dezlow on 29.01.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef DZL_ITERATOR_HPP
#define DZL_ITERATOR_HPP

#include "types.hpp"

namespace dzl
{
    template<typename container>
    class Iterator
    {
        using ValueType = typename container::ValueType;
        using ReferenceType = typename container::ValueType&;
        using PointerType = typename container::ValueType*;
    public: // ctrs
        Iterator(PointerType ptr) : mPtr(ptr) {}
    public: // setters
        Iterator operator++(int);
        Iterator operator--(int);
        Iterator& operator++() { mPtr++; return *this; }
        Iterator& operator--() { mPtr--; return *this; }
    public: // getters
        ReferenceType operator[](size_t index) { return *(mPtr + index); }
        PointerType operator->() { return mPtr; }
        ReferenceType operator*() { return *mPtr; }
        bool operator==(const Iterator& other) { return mPtr == other.mPtr; }
        bool operator!=(const Iterator& other) { return mPtr != other.mPtr; }
    private:
        PointerType mPtr;
    };
    template<typename container>
    Iterator<container> Iterator<container>::operator--(int)
    {
        Iterator it = *this;
        --(*this);
        return it;
    }
    template<typename container>
    Iterator<container> Iterator<container>::operator++(int)
    {
        Iterator it = *this;
        ++(*this);
        return it;
    }
}

#endif //DZL_ITERATOR_HPP
