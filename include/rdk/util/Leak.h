//
// Created by Ruurd Adema on 11/11/2022.
// Copyright (c) 2022 Sound on Digital. All rights reserved.
//

#pragma once

#include <utility>

namespace rdk
{

/**
 * Little class which deliberately leaks a dynamically instantiated object of Type
 * @tparam T
 */
template <class T>
class Leak
{
public:
    template <class... Args>
    explicit Leak (Args&&... args)
    {
        mInstance = new T (std::forward<Args> (args)...);
    }

    /**
     * @return The allocated object, or nullptr if no object is present.
     */
    T* get()
    {
        return mInstance;
    }

private:
    T* mInstance { nullptr };
};

} // namespace rdk
