//
// Created by Ruurd Adema on 28/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include "Subscription.h"
#include "rdk/stl/Vector.h"

#include <memory>

namespace rdk
{

/**
 * List of subscribers which are internally held by a shared list. This allows the subscribers to have a different,
 * arbitrary lifetime than this list.
 * @tparam Type The type of the subscriber.
 */
template <class Type>
class SharedSubscriberList
{
public:
    SharedSubscriberList() = default;

    RDK_DECLARE_NON_COPYABLE (SharedSubscriberList)
    RDK_DECLARE_NON_MOVEABLE (SharedSubscriberList)

    /**
     * Adds given subscriber to the list.
     * @param subscriber Subscriber to add.
     * @return A subscription which will unsubscribe on destruction.
     */
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

    /**
     * Calls all subscribers by calling back the given callback with each subscriber.
     * @param cb The function to call.
     */
    void call (const std::function<void (Type&)>& cb) const
    {
        if (!cb)
            return;

        for (auto* s : *mSharedList)
            cb (*s);
    }

    /**
     * @return The number of subscribers currently in the list.
     */
    [[nodiscard]] size_t getNumSubscribers() const
    {
        return mSharedList->size();
    }

    /**
     * Tests whether given subscriber is part of the internal list.
     * @param subscriber The susbcriber to test.
     * @return True if given subscriber is part of the list, or false if not.
     */
    bool hasSubscriber (Type* subscriber) const
    {
        return std::any_of (mSharedList->begin(), mSharedList->end(), [subscriber] (Type* element) {
            return element == subscriber;
        });
    }

    /**
     * @return Iterator to the beginning of the vector.
     */
    typename std::vector<Type*>::iterator begin()
    {
        return mSharedList->begin();
    }

    /**
     * @return Iterator to the end of the vector.
     */
    typename std::vector<Type*>::iterator end()
    {
        return mSharedList->end();
    }

private:
    std::shared_ptr<std::vector<Type*>> mSharedList { std::make_shared<std::vector<Type*>>() };
};

} // namespace rdk
