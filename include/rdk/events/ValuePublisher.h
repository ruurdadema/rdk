//
// Created by Ruurd Adema on 23/07/2022.
// Copyright (c) Sound on Digital. All rights reserved.
//

#pragma once

#include <optional>

#include "rdk/util/SubscriberList.h"

namespace rdk {

/**
 * Simple class which holds an optional value and a list of subscribers. Whenever the value is set, subscribers are
 * called with the updated value.
 * @tparam DataType The DataType to store and callback with.
 */
template<class DataType>
class ValuePublisher {
  public:
    /**
     * Subscriber for classes which are interested in updated whenever the value changes.
     */
    class Subscriber {
      public:
        virtual ~Subscriber() = default;

        virtual void value_updated(const std::optional<DataType>& updatedValue) = 0;

      protected:
        void subscribe_to_value_publisher(ValuePublisher& publisher) {
            mSubscription.reset();  // Prevent unsubscribing ourselves when subscribing again to the same publisher.

            // Call back with initial state.
            value_updated(publisher.value_);

            mSubscription = publisher.subscriber_list_.subscribe(this);
        }

      private:
        Subscription mSubscription;
    };

    /**
     * Sets a new value.
     * @param newValue The new value to set.
     */
    void set(DataType&& newValue) {
        value_ = std::move(newValue);
        notify_subscribers();
    }

    /**
     * Sets a new value.
     * @param newValue The new value to set.
     */
    void set(const DataType& newValue) {
        value_ = newValue;
        notify_subscribers();
    }

    /**
     * Calls callback which allows caller to update the value.
     * If no value exists, a new value will created.
     */
    void update(const std::function<void(DataType& data)>& callback) {
        if (!callback)
            return;

        if (value_) {
            callback(*value_);
            notify_subscribers();
        } else {
            DataType newValue {};
            callback(newValue);
            set(std::move(newValue));
        }
    }

    /**
     * Clears the value, or doesn nothing if this class doesn't hold a value.
     */
    void reset() {
        if (value_) {
            value_.reset();
            notify_subscribers();
        }
    }

    /**
     * @return The held value, or a default constructed one if no value is held.
     */
    const DataType& get_or_default() const {
        if (value_) {
            return *value_;
        }

        return get_global_const_instance_of_type<DataType>();
    }

    /**
     * @return A reference to the optional value.
     */
    const std::optional<DataType>& get() const {
        return value_;
    }

  private:
    std::optional<DataType> value_ {};
    SubscriberList<Subscriber> subscriber_list_;

    void notify_subscribers() {
        subscriber_list_.call([this](Subscriber& s) { s.value_updated(value_); });
    }
};

}  // namespace rdk
