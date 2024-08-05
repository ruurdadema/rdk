//
// Created by Ruurd Adema on 15/02/2024.
// Copyright (c) 2024 Sound on Digital. All rights reserved.
//

#include "rdk/util/SubscriberList.h"

#include <catch2/catch.hpp>
#include <utility>

class LambdaSubscriber {
  public:
    explicit LambdaSubscriber(std::function<void()> func) : function_(std::move(func)) {}

    void subscribe_to_subscriber_list(rdk::SubscriberList<LambdaSubscriber>& list) {
        subscription_ = list.add(this);
    }

    void unsubscribe() {
        subscription_.reset();
    }

    void callback() const {
        if (function_) {
            function_();
        }
    }

  private:
    rdk::Subscription subscription_;
    std::function<void()> function_;
};

namespace {
constexpr auto kSubscriberA = "subscriberA";
constexpr auto kSubscriberB = "subscriberB";
constexpr auto kSubscriberC = "subscriberC";
}

TEST_CASE("SubscriberList", "[SharedSubscriberList]") {
    rdk::SubscriberList<LambdaSubscriber> subscribers;

    std::vector<std::string> callbacks;

    LambdaSubscriber subscriberA([&] { callbacks.emplace_back(kSubscriberA); });
    LambdaSubscriber subscriberB([&] { callbacks.emplace_back(kSubscriberB); });

    REQUIRE(subscribers.get_num_subscribers() == 0);

    SECTION("Test unsubscribing") {
        {
            LambdaSubscriber subscriberC([&] { callbacks.emplace_back(kSubscriberC); });

            subscriberC.subscribe_to_subscriber_list(subscribers);

            subscribers.call([](const LambdaSubscriber& s) { s.callback(); });

            REQUIRE(callbacks == std::vector<std::string> {kSubscriberC});
        }

        // At this point subscriberC has been unsubscribed

        subscribers.call([](const LambdaSubscriber& s) { s.callback(); });

        REQUIRE(callbacks == std::vector<std::string> {kSubscriberC});
        REQUIRE(subscribers.get_num_subscribers() == 0);
    }

    SECTION("Test double subscribing") {
        {
            LambdaSubscriber subscriberC([&]() { callbacks.emplace_back(kSubscriberC); });

            subscriberC.subscribe_to_subscriber_list(subscribers);
            subscriberC.subscribe_to_subscriber_list(subscribers); // This should do nothing

            subscribers.call([](const LambdaSubscriber& s) { s.callback(); });

            // Only one callback should be called
            REQUIRE(callbacks == std::vector<std::string> {kSubscriberC});
            REQUIRE(subscribers.get_num_subscribers() == 1);
        }
        REQUIRE(subscribers.get_num_subscribers() == 0);
    }

    subscriberA.subscribe_to_subscriber_list(subscribers);
    REQUIRE(subscribers.has_subscriber(&subscriberA));
    REQUIRE(subscribers.get_num_subscribers() == 1);

    SECTION("Callback once") {
        subscribers.call([](const LambdaSubscriber& s) { s.callback(); });
        REQUIRE(callbacks == std::vector<std::string> {kSubscriberA});
    }

    subscriberB.subscribe_to_subscriber_list(subscribers);
    REQUIRE(subscribers.has_subscriber(&subscriberB));
    REQUIRE(subscribers.get_num_subscribers() == 2);

    SECTION("Callback once with both subscribers") {
        subscribers.call([](const LambdaSubscriber& s) { s.callback(); });
        REQUIRE(callbacks == std::vector<std::string> {kSubscriberA, kSubscriberB});
    }

    SECTION("Callback twice with both subscribers") {
        subscribers.call([](const LambdaSubscriber& s) { s.callback(); });
        subscribers.call([](const LambdaSubscriber& s) { s.callback(); });
        REQUIRE(callbacks == std::vector<std::string> {kSubscriberA, kSubscriberB, kSubscriberA, kSubscriberB});
    }

    SECTION("Callback excluding subscriber") {
        subscribers.call([](const LambdaSubscriber& s) { s.callback(); }, &subscriberA);
        REQUIRE(callbacks == std::vector<std::string> {kSubscriberB});
    }

    subscriberA.unsubscribe();
    REQUIRE(subscribers.get_num_subscribers() == 1);
    REQUIRE(subscribers.has_subscriber(&subscriberA) == false);

    subscriberB.unsubscribe();
    REQUIRE(subscribers.get_num_subscribers() == 0);
}
