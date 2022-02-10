//
// Created by Dezlow on 29.01.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef DZL_VECTOR_HPP
#define DZL_VECTOR_HPP

#include <initializer_list>
#include <algorithm>
#include "iterator.hpp"

namespace dzl
{
    template<typename T>
    class vector
    {
        using It = Iterator<vector<T>>;
    public: // ctrs
        vector();
        vector(const std::initializer_list <T>& data);
        vector(vector& other);
    public: // dtrs
        ~vector();
    public: // setters
        void push_back(T&& val);
        void push_back(const T& val);
        void pop_back();
        void clear();
        template<class... Args>
        T& emplace_back(Args&& ... args);
    public: // getters
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        bool operator==(const vector<T>& other) const;
        bool operator!=(const vector<T>& other) const;
        size_t size() const;
        size_t capacity() const;
        T& at(size_t index);
        It begin();
        It end();
        It cbegin() const;
        It cend() const;
    public: // other
        using ValueType = T;
    private:
        void ReAlloc(size_t newCapacity);

        T* mData = nullptr;
        size_t mSize{};
        size_t mCapacity{};
    };

    template<class T>
    dzl::vector<T>::vector() { ReAlloc(2); }

    template<class T>
    dzl::vector<T>::~vector() { clear(); ::operator delete(mData, mCapacity * sizeof(T)); }

    template<class T>
    void dzl::vector<T>::push_back(T&& val) { emplace_back(std::move(val)); }

    template<class T>
    void dzl::vector<T>::push_back(const T& val)
    {
        if (mSize >= mCapacity)
            ReAlloc(mCapacity + mCapacity / 2);
        mData[mSize] = val;
        mSize++;
    }

    template<class T>
    size_t dzl::vector<T>::capacity() const { return mCapacity; }

    template<class T>
    size_t dzl::vector<T>::size() const { return mSize; }

    template<class T>
    void dzl::vector<T>::ReAlloc(size_t newCapacity)
    {
        T* newBlock = (T*) ::operator new(newCapacity * sizeof(T));

        if (newCapacity < mSize)
            mSize = newCapacity;

        for (size_t i = 0; i < mSize; ++i)
            new(&newBlock[i]) T(std::move(mData[i]));

        for (size_t i = 0; i < mSize; ++i)
            mData[i].~T();

        ::operator delete(mData, mCapacity * sizeof(T));
        mData = newBlock;
        mCapacity = newCapacity;
    }

    template<class T>
    void dzl::vector<T>::clear()
    {
        for (size_t i = 0; i < mSize; ++i)
            mData[i].~T();
        mSize = 0;
    }

    template<class T>
    void dzl::vector<T>::pop_back()
    {
        if (mSize > 0)
        {
            mSize--;
            mData[mSize].~T();
        }
    }

    template<class T>
    const T& dzl::vector<T>::operator[](size_t index) const { return mData[index]; }

    template<class T>
    T& dzl::vector<T>::operator[](size_t index) { return mData[index]; }

    template<class T>
    typename dzl::vector<T>::It dzl::vector<T>::begin() { return It(mData); }

    template<class T>
    typename dzl::vector<T>::It dzl::vector<T>::end() { return It(mData + mSize); }

    template<class T>
    typename dzl::vector<T>::It dzl::vector<T>::cbegin() const { return It(mData); }

    template<class T>
    typename dzl::vector<T>::It dzl::vector<T>::cend() const { return It(mData[mSize]); }

    template<class T>
    dzl::vector<T>::vector(vector& other)
    {
        T* newBlock = (T*) ::operator new(other.mCapacity * sizeof(T));

        for (int i = 0; i < other.mSize; ++i)
            new(&newBlock[i]) T(std::move(other.mData[i]));

        mData = newBlock;
        mCapacity = other.mCapacity;
        mSize = other.mSize;
    }

    template<class T>
    T& dzl::vector<T>::at(size_t index) { return mData[index]; }

    template<typename T>
    template<class... Args>
    T& vector<T>::emplace_back(Args&& ... args)
    {
        if (mSize >= mCapacity)
            ReAlloc(mCapacity + mCapacity / 2);

        mData[mSize] = T(std::forward<Args>(args)...);
        return mData[mSize++];
    }

    template<typename T>
    vector<T>::vector(const std::initializer_list <T>& data) : mSize(data.size()), mCapacity(data.size() + 5)
    {
        mData = new T[mSize];
        for (size_t i = 0; i < mSize; ++i)
            mData[i] = *(data.begin() + i);
    }

    template<typename T>
    bool vector<T>::operator==(const vector <T>& other) const
    {
        if (other.mSize != mSize || other.mCapacity != mCapacity) return false;

        for (size_t i = 0; i < mSize; ++i)
            if (mData[i] != other[i])
                return false;

        return true;
    }

    template<typename T>
    bool vector<T>::operator!=(const vector <T>& other) const { return !operator==(other); }
}

#endif //DZL_VECTOR_HPP
