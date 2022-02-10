//
// Created by Dezlow on 29.01.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef DZL_STRING_HPP
#define DZL_STRING_HPP

#include "types.hpp"
#include <algorithm>
#include <ostream>

namespace dzl
{
    constexpr size_t strlen(const char* data)
    {
        size_t i = 0;
        while (data[i] != '\0')
        {
            i++;
        }
        return i;
    }

    class string
    {
    public: // ctrs
        string() { mData = new char[1]; mData[0] = '\0'; }
        string(const char* str)
        {
            if (str == nullptr) {
                mData = new char[1]; mData[0] = '\0';
            }
            else {
                MoveStr(str);
            }
        }
        string(const string& str) { MoveStr(str); }
    public: // dtrs
        ~string() { delete mData; }
    public: // setters
        string& operator=(const char* str) { MoveStr(str); return *this; }
        string& operator+(const char* str)
        {
            size_t len = strlen(mData) + strlen(str);
            char* temp = new char[len + 1];
            for (unsigned j=0; j < strlen(mData); j++)
                temp[j] = mData[j];
            for (unsigned i=0; i < strlen(str); i++)
                temp[strlen(mData)+i] = str[i];
            delete mData;
            temp[len] = '\0';
            mData = temp;
            return *this;
        }
        string& operator=(const string& str) { MoveStr(str); return *this; }
        string& operator+(const string& str)
        {
            size_t len = strlen(mData) + strlen(str);
            char* temp = new char[len + 1];
            for (unsigned j=0; j < strlen(mData); j++)
                temp[j] = mData[j];
            for (unsigned i=0; i < strlen(str); i++)
                temp[strlen(mData)+i] = str[i];
            delete mData;
            temp[len] = '\0';
            mData = temp;
            return *this;
        }
    public: // getters
        bool operator==(const char* other)
        {
            size_t size = strlen(other);

            if (strlen(mData) < size)
                return false;

            for (size_t i = 0; i < size; ++i)
            {
                if (mData[i] == other[i])
                    return true;
            }

            return false;
        }
        bool operator==(const string& other) { return operator==(other); }
        bool operator!=(const char* other) { return !operator==(other); }
        bool operator!=(const string& other) { return !operator==(other); }

        operator const char*() const { return mData; }
        operator const char*() { return mData; }

    public: // other
        friend std::ostream& operator<<(std::ostream& os, const string& str) { os << str.mData; return os; }
    private:
        void MoveStr(const char* str)
        {
            size_t s = strlen(str);
            mData = new char[s];
            for (size_t i = 0; i < s; ++i)
            {
                mData[i] = str[i];
            }
            mData[s] = '\0';
        }

        char* mData{};
    };
}

#endif //DZL_STRING_HPP
