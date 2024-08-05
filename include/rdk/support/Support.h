//
// Created by Ruurd Adema on 06/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

namespace rdk {

/**
 * This function returns a reference to a read/write, default-constructed static object of type T. There will be exactly
 * one of these objects present per instantiated type, per process.
 */
template<typename T>
T& get_global_instance_of_type() {
    static T _defaultObject;
    return _defaultObject;
}

/**
 * This function returns a reference to a read-only, default-constructed static object of type T. There will be exactly
 * one of these objects present per instantiated type, per process.
 */
template<typename T>
const T& get_global_const_instance_of_type() {
    const static T _defaultObject;
    return _defaultObject;
}

/**
 * Updates value and returns whether the value was changed.
 * @tparam T Type of the method.
 * @param valueToUpdate The value to update.
 * @param newValue New value to assign to the value to update.
 * @return True if the value changed, or false if not.
 */
template<class T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
bool update(T& valueToUpdate, const T& newValue) {
    if (valueToUpdate != newValue) {
        valueToUpdate = newValue;
        return true;
    }
    return false;
}

}  // namespace rdk
