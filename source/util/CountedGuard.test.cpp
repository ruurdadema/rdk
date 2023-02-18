//
// Created by Ruurd Adema on 01/02/2022.
// Copyright (c) Owllab. All rights reserved.
//

#include "catch.hpp"

#include "rdk/util/CountedGuard.h"

TEST_CASE ("ScopedGuard default constructor", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};
    int numberOfTimesOnReleaseCalled = 0;
    counter.onRelease = [&] {
        numberOfTimesOnReleaseCalled++;
    };

    {
        auto a = counter.getGuard();
        REQUIRE (counter.getCount() == 1);

        auto b = a; // NOLINT
        REQUIRE (counter.getCount() == 2);
    }
    REQUIRE (counter.getCount() == 0);
    REQUIRE (numberOfTimesOnReleaseCalled == 1);
}

TEST_CASE ("ScopedGuard copy constructor", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);
        {
            auto b = a; // NOLINT
            REQUIRE (counter.getCount() == 2);
        }
        REQUIRE (counter.getCount() == 1);
    }
    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard move constructor", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);
        {
            auto b = std::move (a);
            REQUIRE (counter.getCount() == 1);
        }

        REQUIRE (counter.getCount() == 0); // a moved to b, which makes the scope of b decrease the count already.
    }
    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard move constructor with lambda", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);
        {
            auto func = [&, b { std::move (a) }]() {
                REQUIRE (counter.getCount() == 1);
            };
            func();

            REQUIRE (counter.getCount() == 1);
        }
        REQUIRE (counter.getCount() == 0);
    }
    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard move assignment", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);
        {
            auto b = counter.getGuard();
            REQUIRE (counter.getCount() == 2);

            b = std::move (a);
            REQUIRE (counter.getCount() == 1);
        }
        REQUIRE (counter.getCount() == 0);
    }

    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard move assignment with lambda", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);
        {
            auto func = [&, b = std::move (a)]() {
                REQUIRE (counter.getCount() == 1);
            };

            func();

            REQUIRE (counter.getCount() == 1);
        }
        REQUIRE (counter.getCount() == 0);
    }

    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard copy assignment", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);
        {
            auto b = a; // NOLINT

            REQUIRE (counter.getCount() == 2);
        }
        REQUIRE (counter.getCount() == 1);
    }
    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard copy assignment lambda", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);

        {
            auto func = [&, b = a]() {
                REQUIRE (counter.getCount() == 2);
            };

            func();

            REQUIRE (counter.getCount() == 2);
        }
        REQUIRE (counter.getCount() == 1);
    }
    REQUIRE (counter.getCount() == 0);
}

TEST_CASE ("ScopedGuard copy construction lambda", "[ScopedGuard]")
{
    rdk::CountedGuard counter {};

    {
        auto a = counter.getGuard();

        REQUIRE (counter.getCount() == 1);

        {
            auto func = [&, b (a)]() {
                REQUIRE (counter.getCount() == 2);
            };

            func();

            REQUIRE (counter.getCount() == 2);
        }

        REQUIRE (counter.getCount() == 1);
    }
    REQUIRE (counter.getCount() == 0);
}
