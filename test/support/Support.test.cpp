//
// Created by Ruurd Adema on 05/08/2024.
// Copyright (c) 2024 Sound on Digital. All rights reserved.
//

#include "rdk/support/Support.h"

#include <catch2/catch_all.hpp>

namespace {
class InstanceCounted {
  public:
    InstanceCounted() {
        times_constructed_++;
    }

    ~InstanceCounted() {
        times_destructed_++;
    }

    [[nodiscard]] static int num_alive() {
        return times_constructed_ - times_destructed_;
    }

  private:
    inline static int times_constructed_ {0};
    inline static int times_destructed_ {0};
};

}  // namespace

TEST_CASE("get_global_instance_of_type", "[Support]") {
    SECTION("Test CountedInstantiation") {
        REQUIRE(InstanceCounted::num_alive() == 0);

        {
            InstanceCounted instance;
            REQUIRE(InstanceCounted::num_alive() == 1);
        }

        REQUIRE(InstanceCounted::num_alive() == 0);
    }

    SECTION("Test get_global_instance_of_type") {
        REQUIRE(InstanceCounted::num_alive() == 0);
        {
            rdk::get_global_instance_of_type<InstanceCounted>();
            REQUIRE(InstanceCounted::num_alive() == 1);
        }
        REQUIRE(InstanceCounted::num_alive() == 1);
    }

    SECTION("Test get_global_const_instance_of_type") {
        REQUIRE(InstanceCounted::num_alive() == 1);
        {
            rdk::get_global_const_instance_of_type<InstanceCounted>();
            REQUIRE(InstanceCounted::num_alive() == 2);
        }
        REQUIRE(InstanceCounted::num_alive() == 2);
    }
}

TEST_CASE("update", "[Support]") {
    SECTION("Test update") {
        int value = 0;

        REQUIRE(rdk::update(value, 1));
        REQUIRE(value == 1);

        REQUIRE_FALSE(rdk::update(value, 1));
        REQUIRE(value == 1);
    }
}
