//
// Created by Ruurd Adema on 23/07/2022.
// Copyright (c) Sound on Digital. All rights reserved.
//

#pragma once

#include "rdk/util/SharedSubscriberList.h"
#include <optional>

namespace rdk
{

/**
 * Simple class which holds an optional value and a list of subscribers. Whenever the value is set, subscribers are
 * called with the updated value.
 * @tparam DataType The DataType to store and callback with.
 */
template <class DataType>
class ValuePublisher
{
public:
    /**
     * Subscriber for classes which are interested in updated whenever the value changes.
     */
    class Subscriber
    {
    public:
        virtual void valueUpdated (const std::optional<DataType>& updatedValue) = 0;

    protected:
        void subscribeToValuePublisher (ValuePublisher<DataType>& publisher)
        {
            mSubscription.reset(); // Prevent unsubscribing ourselves when subscribing again to the same publisher.

            // Call back with initial state.
            valueUpdated (publisher.mValue);

            mSubscription = publisher.mSubscriberList.subscribe (this);
        }

    private:
        rdk::Subscription mSubscription;
    };

    /**
     * Sets a new value.
     * @param newValue The new value to set.
     */
    void set (DataType&& newValue)
    {
        mValue = std::move (newValue);
        notifySubscribers();
    }

    /**
     * Sets a new value.
     * @param newValue The new value to set.
     */
    void set (const DataType& newValue)
    {
        mValue = newValue;
        notifySubscribers();
    }

    /**
     * Calls callback which allows caller to update the value.
     * If no value exists, a new value will created.
     */
    void update (const std::function<void (DataType& data)>& callback)
    {
        if (!callback)
            return;

        if (mValue)
        {
            callback (*mValue);
            notifySubscribers();
        }
        else
        {
            DataType newValue {};
            callback (newValue);
            set (std::move (newValue));
        }
    }

    /**
     * Clears the value, if there is one.
     */
    void reset()
    {
        if (mValue)
        {
            mValue.reset();
            notifySubscribers();
        }
    }

    /**
     * @return The held value, or a default constructed one if no value is held.
     */
    const DataType& getOrDefault() const
    {
        if (mValue)
            return *mValue;

        return getDefaultObjectForType<DataType>();
    }

    /**
     * @return A reference to the optional value.
     */
    const std::optional<DataType>& get() const
    {
        return mValue;
    }

private:
    std::optional<DataType> mValue {};
    rdk::SharedSubscriberList<Subscriber> mSubscriberList;

    void notifySubscribers()
    {
        mSubscriberList.call ([this] (Subscriber& s) {
            s.valueUpdated (mValue);
        });
    }
};

} // namespace rdk
