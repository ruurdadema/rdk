//
// Created by Ruurd Adema on 28/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <algorithm>

#include "Subscription.h"
#include "rdk/detail/NonCopyable.h"
#include "rdk/detail/NonMoveable.h"

namespace rdk {

/**
 * List of subscribers which are internally held by a shared list. This allows the subscribers to have a different,
 * arbitrary lifetime than this list.
 * @tparam Type The type of the subscriber.
 */
template<class Type>
class SubscriberList {
  public:
    SubscriberList() = default;

    RDK_DECLARE_NON_COPYABLE(SubscriberList)
    RDK_DECLARE_NON_MOVEABLE(SubscriberList)

    /**
     * Adds given subscriber to the list.
     * @param subscriber Subscriber to add.
     * @return A subscription which will unsubscribe on destruction.
     */
    Subscription add(Type* subscriber) {
        if (subscriber == nullptr)
            return {};

        // Find an existing entry for subscriber.
        for (auto& s : *subscribers_) {
            if (s.subscriber == subscriber) {
                s.count += 1;
                return rdk::Subscription([subscribers = subscribers_, subscriber] {
                    SubscriberList::unsubscribe(subscribers, subscriber);
                });
            }
        }

        // No existing entry was found, so create a new one.
        subscribers_->push_back(SubscriberAndCount {subscriber, 1});

        return Subscription([subscribers = subscribers_, subscriber] {
            SubscriberList::unsubscribe(subscribers, subscriber);
        });
    }

    /**
     * Calls all subscribers by calling back the given callback with each subscriber.
     * @param cb The function to call.
     * @param excluding If given, this subscriber will not be called.
     */
    void call(const std::function<void(Type&)>& cb, Type* excluding = nullptr) const {
        if (!cb) {
            return;
        }

        for (auto& s : *subscribers_) {
            if (s.subscriber != excluding) {
                cb(*s.subscriber);
            }
        }
    }

    /**
     * @return The number of subscribers currently in the list.
     */
    [[nodiscard]] size_t get_num_subscribers() const {
        return subscribers_->size();
    }

    /**
     * Tests whether given subscriber is part of the internal list.
     * @param subscriber The susbcriber to test.
     * @return True if given subscriber is part of the list, or false if not.
     */
    bool has_subscriber(Type* subscriber) const {
        return std::any_of(subscribers_->begin(), subscribers_->end(), [subscriber](SubscriberAndCount& element) {
            return element.subscriber == subscriber;
        });
    }

    /**
     * @return Iterator to the beginning of the vector.
     */
    typename std::vector<Type*>::iterator begin() {
        return subscribers_->begin();
    }

    /**
     * @return Iterator to the end of the vector.
     */
    typename std::vector<Type*>::iterator end() {
        return subscribers_->end();
    }

  private:
    struct SubscriberAndCount {
        Type* subscriber;
        size_t count;
    };

    std::shared_ptr<std::vector<SubscriberAndCount>> subscribers_ {std::make_shared<std::vector<SubscriberAndCount>>()};

    static void
    unsubscribe(const std::shared_ptr<std::vector<SubscriberAndCount>>& subscribers, Type* subscriberToRemove) {
        subscribers->erase(
            std::remove_if(
                subscribers->begin(),
                subscribers->end(),
                [subscriberToRemove](SubscriberAndCount& sc) {
                    if (sc.subscriber == subscriberToRemove) {
                        if (sc.count <= 1)
                            return true;
                        --sc.count;
                    }
                    return false;
                }
            ),
            subscribers->end()
        );
    }
};

}  // namespace rdk
