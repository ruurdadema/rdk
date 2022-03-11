//
// Created by Ruurd Adema on 28/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include "Subscription.h"
#include "rdk/stl/Vector.h"

namespace rdk
{

template <class Type> class SharedSubscriberList
{
public:
    rdk::Subscription subscribe (Type* subscriber)
    {
        if (subscriber == nullptr)
            return {};

        if (rdk::addIfNotAlreadyThere (*mSharedList, subscriber))
        {
            return Subscription ([sharedList { mSharedList }, subscriber] {
                rdk::remove (*sharedList, subscriber);
            });
        }

        return {};
    }

    void call (const std::function<void (Type&)>& cb) const
    {
        if (!cb)
            return;

        for (auto* s : *mSharedList)
            cb (*s);
    }

private:
    std::shared_ptr<std::vector<Type*>> mSharedList { std::make_shared<std::vector<Type*>>() };
};

} // namespace rdk
