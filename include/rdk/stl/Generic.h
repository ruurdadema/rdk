//
// Created by Ruurd Adema on 28/10/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include "../support/Support.h"

namespace rdk
{

// Borrowed from: https://stackoverflow.com/a/16597048/2576876 (and adjusted a bit)
template <typename ContainerT, typename PredicateT> size_t eraseIf (ContainerT& items, const PredicateT& predicate)
{
    size_t numErased = 0;

    for (auto it = items.begin(); it != items.end();)
    {
        if (predicate (*it))
        {
            it = items.erase (it);
            numErased++;
        }
        else
        {
            ++it;
        }
    }
    return numErased;
}

template <typename Container> typename Container::value_type* get (Container& container, size_t index)
{
    if (index < container.size())
        return &container[index];
    return nullptr;
}

template <typename Container> const typename Container::value_type* get (const Container& container, size_t index)
{
    if (index < container.size())
        return &container[index];
    return nullptr;
}

template <typename Container>
const typename Container::value_type& getWithDefault (const Container& container, size_t index)
{
    if (index < container.size())
        return container[index];
    return rdk::getDefaultObjectForType<typename Container::value_type>();
}

template <typename Container>
const typename Container::mapped_type* find (const Container& container, const typename Container::key_type& key)
{
    auto output = container.find (key);
    if (output == container.end())
        return nullptr;
    return &output->second;
}

template <typename Container>
const typename Container::mapped_type& find_with_default (
    const Container& container,
    const typename Container::key_type& key)
{
    auto output = container.find (key);
    if (output == container.end())
        return rdk::getDefaultObjectForType<Container::mapped_type>();
    return output->second;
}

} // namespace rdk
