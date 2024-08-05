//
// Created by Ruurd Adema on 12/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#include "rdk/util/ScopedAtomicCounter.h"

#include <catch2/catch.hpp>

TEST_CASE("Previous value", "[ScopedAtomicCounter]") {
    std::atomic<int> atomic {};

    {
        const ScopedAtomicCounter a(atomic);
        REQUIRE(a.previousValue() == 0);

        REQUIRE(atomic.load() == 1);

        const ScopedAtomicCounter b(atomic);
        REQUIRE(b.previousValue() == 1);
    }
    REQUIRE(atomic.load() == 0);
}

TEST_CASE("Copy constructor", "[ScopedAtomicCounter]") {
    std::atomic<int> atomic {};

    {
        const ScopedAtomicCounter a(atomic);

        REQUIRE(atomic.load() == 1);
        {
            const ScopedAtomicCounter b(a);
            REQUIRE(b.previousValue() == 1);

            REQUIRE(atomic.load() == 2);
        }
        REQUIRE(atomic.load() == 1);
    }
    REQUIRE(atomic.load() == 0);
}

TEST_CASE("Copy assignment", "[ScopedAtomicCounter]") {
    std::atomic<int> atomic {};

    {
        const ScopedAtomicCounter a(atomic);

        REQUIRE(atomic.load() == 1);
        {
            const ScopedAtomicCounter b = a;
            REQUIRE(b.previousValue() == 1);

            REQUIRE(atomic.load() == 2);
        }
        REQUIRE(atomic.load() == 1);
    }
    REQUIRE(atomic.load() == 0);
}

TEST_CASE("Copy assignment lambda", "[ScopedAtomicCounter]") {
    std::atomic<int> atomic {};

    {
        ScopedAtomicCounter a(atomic);
        REQUIRE(atomic.load() == 1);

        {
            auto func = [b = a, &atomic] {
                REQUIRE(b.previousValue() == 1);
                REQUIRE(atomic.load() == 2);
            };

            func();

            REQUIRE(atomic.load() == 2);
        }
        REQUIRE(atomic.load() == 1);
    }
    REQUIRE(atomic.load() == 0);
}

TEST_CASE("Copy construction lambda", "[ScopedAtomicCounter]") {
    std::atomic<int> atomic {};

    {
        ScopedAtomicCounter a(atomic);
        REQUIRE(atomic.load() == 1);

        {
            auto func = [b(a), &atomic]() {
                REQUIRE(b.previousValue() == 1);
                REQUIRE(atomic.load() == 2);
            };

            func();

            REQUIRE(atomic.load() == 2);
        }

        REQUIRE(atomic.load() == 1);
    }
    REQUIRE(atomic.load() == 0);
}
