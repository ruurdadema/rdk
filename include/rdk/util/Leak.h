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
 * You probably shouldn't use this, obviously, but in certain rare cases it can be helpful to leak memory. When that is
 * the case it is better to make clear that this is intentional by using this class.
 * @tparam T
 */
template <class T>
class Leak
{
public:
    template <class... Args>
    explicit Leak (Args&&... args)
    {
        instance_ = new T (std::forward<Args> (args)...);
    }

    /**
     * @return The allocated object, or nullptr if no object is present.
     */
    T* get()
    {
        return instance_;
    }

private:
    T* instance_ { nullptr };
};

} // namespace rdk
