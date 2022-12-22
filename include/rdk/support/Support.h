//
// Created by Ruurd Adema on 06/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <numeric>
#include <type_traits>

/**
 * Shortcut for declaring a class non copyable.
 */
#define RDK_DECLARE_NON_COPYABLE(className)                                                                            \
    className (const className&) = delete;                                                                             \
    className& operator= (const className&) = delete;

/**
 * Shortcut for declaring a class non moveable.
 */
#define RDK_DECLARE_NON_MOVEABLE(className)                                                                            \
    className (className&&) = delete;                                                                                  \
    className& operator= (className&&) = delete;

namespace rdk
{

static constexpr size_t InvalidIndex = std::numeric_limits<size_t>::max();

template <class T> static T invalidIndex()
{
    return std::numeric_limits<T>::max();
}

/**
 * Converts an enum class to its underlying type.
 * @tparam T The type to convert.
 * @param e The value to convert.
 * @return The underlying value.
 */
template <typename T> auto toIntegral (T e)
{
    return static_cast<std::underlying_type_t<T>> (e);
}

/**
 * This function returns a reference to a read-only, default-constructed static object of type T. There will be exactly
 * one of these objects present per instantiated type, per process.
 */
template <typename T> const T& getDefaultObjectForType()
{
    const static T _defaultObject;
    return _defaultObject;
}

/**
 * This function returns a reference to a read/write, default-constructed static object of type T. There will be exactly
 * one of these objects present per instantiated type, per process.
 */
template <typename T> T& getGlobalObjectForType()
{
    static T _defaultObject;
    return _defaultObject;
}

/**
 * Updates value and returns whether the value was changed.
 * @tparam T Type of the method.
 * @param valueToUpdate The value to update.
 * @param newValue New value to assign to the value to update.
 * @return True if the value changed, or false if not.
 */
template <class T> bool update (T& valueToUpdate, const T& newValue)
{
    if (valueToUpdate != newValue)
    {
        valueToUpdate = newValue;
        return true;
    }
    return false;
}

} // namespace rdk
