//
// Created by Ruurd Adema on 15/02/2024.
// Copyright (c) 2024 Sound on Digital. All rights reserved.
//

#include <catch2/catch.hpp>
#include <utility>

#include "rdk/util/SharedSubscriberList.h"

class LambdaSubscriber
{
public:
    explicit LambdaSubscriber (std::function<void()> func) : mFunction (std::move (func)) {}

    void subscribeToSubscriberList (rdk::SharedSubscriberList<LambdaSubscriber>& list)
    {
        mSubscription = list.subscribe (this);
    }

    void unsubscribe()
    {
        mSubscription.reset();
    }

    void callback()
    {
        if (mFunction)
            mFunction();
    }

private:
    rdk::Subscription mSubscription;
    std::function<void()> mFunction;
};

TEST_CASE ("", "[SharedSubscriberList]")
{
    rdk::SharedSubscriberList<LambdaSubscriber> subscribers;

    std::vector<std::string> callbacks;

    LambdaSubscriber subscriberA ([&]() {
        callbacks.emplace_back ("subscriberA");
    });

    LambdaSubscriber subscriberB ([&]() {
        callbacks.emplace_back ("subscriberB");
    });

    REQUIRE (subscribers.getNumSubscribers() == 0);

    SECTION ("Test unsubscribing")
    {
        {
            LambdaSubscriber subscriberC ([&]() {
                callbacks.emplace_back ("subscriberC");
            });

            subscriberC.subscribeToSubscriberList (subscribers);

            subscribers.call ([] (LambdaSubscriber& s) {
                s.callback();
            });

            REQUIRE (callbacks == std::vector<std::string> { "subscriberC" });
        }

        // At this point subscriberC has been unsubscribed

        subscribers.call ([] (LambdaSubscriber& s) {
            s.callback();
        });

        REQUIRE (callbacks == std::vector<std::string> { "subscriberC" });
        REQUIRE (subscribers.getNumSubscribers() == 0);
    }

    SECTION ("Test double subscribing")
    {
        {
            LambdaSubscriber subscriberC ([&]() {
                callbacks.emplace_back ("subscriberC");
            });

            subscriberC.subscribeToSubscriberList (subscribers);
            subscriberC.subscribeToSubscriberList (subscribers);

            subscribers.call ([] (LambdaSubscriber& s) {
                s.callback();
            });

            REQUIRE (callbacks == std::vector<std::string> { "subscriberC" });
            REQUIRE (subscribers.getNumSubscribers() == 1);
        }
        REQUIRE (subscribers.getNumSubscribers() == 0);
    }

    subscriberA.subscribeToSubscriberList (subscribers);
    REQUIRE (subscribers.hasSubscriber (&subscriberA));
    REQUIRE (subscribers.getNumSubscribers() == 1);

    SECTION ("Callback once")
    {
        subscribers.call ([] (LambdaSubscriber& s) {
            s.callback();
        });

        REQUIRE (callbacks == std::vector<std::string> { "subscriberA" });
    }

    subscriberB.subscribeToSubscriberList (subscribers);
    REQUIRE (subscribers.hasSubscriber (&subscriberB));
    REQUIRE (subscribers.getNumSubscribers() == 2);

    SECTION ("Callback once with both subscribers")
    {
        subscribers.call ([] (LambdaSubscriber& s) {
            s.callback();
        });

        REQUIRE (callbacks == std::vector<std::string> { "subscriberA", "subscriberB" });
    }

    SECTION ("Callback twice with both subscribers")
    {
        subscribers.call ([] (LambdaSubscriber& s) {
            s.callback();
        });

        subscribers.call ([] (LambdaSubscriber& s) {
            s.callback();
        });

        REQUIRE (callbacks == std::vector<std::string> { "subscriberA", "subscriberB", "subscriberA", "subscriberB" });
    }

    subscriberA.unsubscribe();
    REQUIRE (subscribers.getNumSubscribers() == 1);
    REQUIRE (subscribers.hasSubscriber(&subscriberA) == false);

    subscriberB.unsubscribe();
    REQUIRE (subscribers.getNumSubscribers() == 0);
}
