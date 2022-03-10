//
// Created by Ruurd Adema on 27/10/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <limits>
#include <rdk/stl/Generic.h>
#include <rdk/support/Support.h>

namespace rdk
{

/**
 * Simple utility class holding a std::vector and adding all kinds of handy convenience functions.
 * @tparam Type Type of the element.
 */
template <class Type> class Vector
{
public:
    std::vector<Type> inner;

    bool addIfNotAlreadyThere (Type elementToAdd)
    {
        for (auto& element : inner)
            if (element == elementToAdd)
                return false;
        inner.push_back (std::move (elementToAdd));
        return true;
    }

    bool contains (const Type elementToFind)
    {
        for (const auto& element : inner)
            if (element == elementToFind)
                return true;
        return false;
    }

    bool remove (Type elementToRemove)
    {
        return eraseIf (inner, [elementToRemove] (Type element) {
                   return element == elementToRemove;
               }) > 0;
    }

    size_t indexOf (const Type elementToFind)
    {
        for (size_t i = 0; i < inner.size(); ++i)
            if (inner[i] == elementToFind)
                return i;
        return InvalidIndex;
    }

    void ensureIndexExists (size_t indexToExist)
    {
        if (indexToExist <= inner.size())
            inner.resize (indexToExist + 1);
    }
};

template <typename Type> bool addIfNotAlreadyThere (Type& vector, typename Type::value_type&& elementToAdd)
{
    for (auto& element : vector)
        if (element == elementToAdd)
            return false;
    vector.push_back (std::move (elementToAdd));
    return true;
}

template <typename Type> bool addIfNotAlreadyThere (Type& vector, const typename Type::value_type& elementToAdd)
{
    for (auto& element : vector)
        if (element == elementToAdd)
            return false;
    vector.push_back (std::move (elementToAdd));
    return true;
}

template <typename Type> bool contains (const Type& vector, const typename Type::value_type& elementToFind)
{
    for (const auto& element : vector)
        if (element == elementToFind)
            return true;
    return false;
}

template <typename Type> bool contains (const Type& array, std::string_view elementToFind)
{
    for (auto& element : array)
        if (element == elementToFind)
            return true;
    return false;
}

template <typename Type> bool remove (Type& vector, const typename Type::value_type& elementToRemove)
{
    return eraseIf (vector, [elementToRemove] (typename Type::value_type element) {
               return element == elementToRemove;
           }) > 0;
}

template <typename Type> size_t indexOf (const std::vector<Type>& vector, const Type elementToFind)
{
    for (size_t i = 0; i < vector.size(); ++i)
        if (vector[i] == elementToFind)
            return i;
    return InvalidIndex;
}

/**
 * Ensures given index exists, increasing the size if necessary.
 * @tparam Type Element type.
 * @param vector Vector to resize.
 * @param indexToExist The index which should exist.
 * @return The element pointed to by index.
 */
template <typename Type> Type& ensureIndexExists (std::vector<Type>& vector, size_t indexToExist)
{
    if (indexToExist >= vector.size())
        vector.resize (indexToExist + 1);
    return vector[indexToExist];
}

} // namespace rdk
