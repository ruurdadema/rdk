//
// Created by Ruurd Adema on 01/02/2022.
// Copyright (c) Owllab. All rights reserved.
//

#include "rdk/util/CountedGuard.h"

#include "catch2/catch.hpp"

TEST_CASE("ScopedGuard default constructor", "[ScopedGuard]") {
    rdk::CountedGuard counter {};
    int number_of_times_on_release_called = 0;
    counter.on_release = [&] { number_of_times_on_release_called++; };

    {
        const auto a = counter.get_guard();
        REQUIRE(counter.get_count() == 1);

        auto b = a;  // NOLINT
        REQUIRE(counter.get_count() == 2);
    }
    REQUIRE(counter.get_count() == 0);
    REQUIRE(number_of_times_on_release_called == 1);
}

TEST_CASE("ScopedGuard copy constructor", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        const auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);
        {
            auto b = a;  // NOLINT
            REQUIRE(counter.get_count() == 2);
        }
        REQUIRE(counter.get_count() == 1);
    }
    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard move constructor", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);
        {
            auto b = std::move(a);
            REQUIRE(counter.get_count() == 1);
        }

        REQUIRE(counter.get_count() == 0);  // a moved to b, which makes the scope of b decrease the count already.
    }
    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard move constructor with lambda", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);
        {
            auto func = [&, b {std::move(a)}]() { REQUIRE(counter.get_count() == 1); };
            func();

            REQUIRE(counter.get_count() == 1);
        }
        REQUIRE(counter.get_count() == 0);
    }
    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard move assignment", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);
        {
            auto b = counter.get_guard();
            REQUIRE(counter.get_count() == 2);

            b = std::move(a);
            REQUIRE(counter.get_count() == 1);
        }
        REQUIRE(counter.get_count() == 0);
    }

    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard move assignment with lambda", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);
        {
            auto func = [&, b = std::move(a)]() { REQUIRE(counter.get_count() == 1); };

            func();

            REQUIRE(counter.get_count() == 1);
        }
        REQUIRE(counter.get_count() == 0);
    }

    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard copy assignment", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);
        {
            auto b = a;  // NOLINT

            REQUIRE(counter.get_count() == 2);
        }
        REQUIRE(counter.get_count() == 1);
    }
    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard copy assignment lambda", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);

        {
            auto func = [&, b = a]() { REQUIRE(counter.get_count() == 2); };

            func();

            REQUIRE(counter.get_count() == 2);
        }
        REQUIRE(counter.get_count() == 1);
    }
    REQUIRE(counter.get_count() == 0);
}

TEST_CASE("ScopedGuard copy construction lambda", "[ScopedGuard]") {
    rdk::CountedGuard counter {};

    {
        auto a = counter.get_guard();

        REQUIRE(counter.get_count() == 1);

        {
            auto func = [&, b(a)]() { REQUIRE(counter.get_count() == 2); };

            func();

            REQUIRE(counter.get_count() == 2);
        }

        REQUIRE(counter.get_count() == 1);
    }
    REQUIRE(counter.get_count() == 0);
}
