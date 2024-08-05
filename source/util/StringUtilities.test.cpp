//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#include <rdk/util/StringUtilities.h>

#include <catch2/catch.hpp>

TEST_CASE("Test upToFirstOccurrenceOf", "[StringUtilities]") {
    constexpr std::string_view haystack("one test two test three test");

    SECTION("Without substring included") {
        REQUIRE(rdk::up_to_first_occurrence_of(haystack, "test", false) == "one ");
    }
    SECTION("With substring included") {
        REQUIRE(rdk::up_to_first_occurrence_of(haystack, "test", true) == "one test");
    }
    SECTION("With invalid substring") {
        REQUIRE(rdk::up_to_first_occurrence_of(haystack, "bleep", true).empty());
    }
}

TEST_CASE("Test upToFirstNthOccurrenceOf", "[StringUtilities]") {
    std::string_view haystack("/one/two/three/four");

    SECTION("First occurrence") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(1, haystack, "/", false).empty());
        REQUIRE(rdk::up_to_the_nth_occurrence_of(1, haystack, "/", true) == "/");
    }

    SECTION("Second occurrence") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(2, haystack, "/", false) == "/one");
        REQUIRE(rdk::up_to_the_nth_occurrence_of(2, haystack, "/", true) == "/one/");
    }

    SECTION("Third occurrence") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(3, haystack, "/", false) == "/one/two");
        REQUIRE(rdk::up_to_the_nth_occurrence_of(3, haystack, "/", true) == "/one/two/");
    }

    SECTION("Fourth occurrence") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(4, haystack, "/", false) == "/one/two/three");
        REQUIRE(rdk::up_to_the_nth_occurrence_of(4, haystack, "/", true) == "/one/two/three/");
    }

    SECTION("Multi character needle") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(4, haystack, "two", false).empty());
        REQUIRE(rdk::up_to_the_nth_occurrence_of(4, haystack, "two", true).empty());
    }

    SECTION("Empty string") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(1, "", "/", false).empty());
        REQUIRE(rdk::up_to_the_nth_occurrence_of(1, "", "/", true).empty());
    }

    SECTION("Non-empty string with no needle") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(1, "onetwothreefour", "/", false).empty());
        REQUIRE(rdk::up_to_the_nth_occurrence_of(1, "onetwothreefour", "/", true).empty());
    }

    SECTION("Non-empty string with not enough needles needle") {
        REQUIRE(rdk::up_to_the_nth_occurrence_of(2, "/onetwothreefour", "/", false).empty());
        REQUIRE(rdk::up_to_the_nth_occurrence_of(2, "/onetwothreefour", "/", true).empty());
    }
}

TEST_CASE("Test upToLastOccurrenceOf", "[StringUtilities]") {
    constexpr std::string_view haystack("one test two test three test");

    SECTION("Without substring included") {
        REQUIRE(rdk::up_to_last_occurrence_of(haystack, "test", false) == "one test two test three ");
    }

    SECTION("With substring included") {
        REQUIRE(rdk::up_to_last_occurrence_of(haystack, "test", true) == "one test two test three test");
    }

    SECTION("With invalid substring") {
        REQUIRE(rdk::up_to_first_occurrence_of(haystack, "bleep", true).empty());
    }
}

TEST_CASE("Test fromFirstOccurrenceOf", "[StringUtilities]") {
    constexpr std::string_view haystack("one test two test three test");

    SECTION("Without substring included") {
        REQUIRE(rdk::from_first_occurrence_of(haystack, "test", false) == " two test three test");
    }
    SECTION("With substring included") {
        REQUIRE(rdk::from_first_occurrence_of(haystack, "test", true) == "test two test three test");
    }

    SECTION("With invalid substring") {
        REQUIRE(rdk::from_first_occurrence_of(haystack, "bleep", true).empty());
    }
}

TEST_CASE("Test fromNthOccurrenceOf", "[StringUtilities]") {
    std::string_view haystack("/one/two/three/four");

    SECTION("First occurrence") {
        REQUIRE(rdk::from_nth_occurrence_of(1, haystack, "/", false) == "one/two/three/four");
        REQUIRE(rdk::from_nth_occurrence_of(1, haystack, "/", true) == "/one/two/three/four");
    }

    SECTION("Fourth occurrence") {
        REQUIRE(rdk::from_nth_occurrence_of(4, haystack, "/", false) == "four");
        REQUIRE(rdk::from_nth_occurrence_of(4, haystack, "/", true) == "/four");
    }

    SECTION("Multi character needle") {
        REQUIRE(rdk::from_nth_occurrence_of(1, haystack, "two", false) == "/three/four");
        REQUIRE(rdk::from_nth_occurrence_of(1, haystack, "two", true) == "two/three/four");
    }

    SECTION("Empty string") {
        REQUIRE(rdk::from_nth_occurrence_of(1, "", "/", false).empty());
        REQUIRE(rdk::from_nth_occurrence_of(1, "", "/", true).empty());
    }

    SECTION("Non-empty string with no needle") {
        REQUIRE(rdk::from_nth_occurrence_of(1, "onetwothreefour", "/", false).empty());
        REQUIRE(rdk::from_nth_occurrence_of(1, "onetwothreefour", "/", true).empty());
    }

    SECTION("Non-empty string with not enough needles needle") {
        REQUIRE(rdk::from_nth_occurrence_of(2, "/onetwothreefour", "/", false).empty());
        REQUIRE(rdk::from_nth_occurrence_of(2, "/onetwothreefour", "/", true).empty());
    }
}

TEST_CASE("Test fromLastOccurrenceOf", "[StringUtilities]") {
    constexpr std::string_view haystack("one test two test three test");

    SECTION("Without substring included") {
        REQUIRE(rdk::from_last_occurrence_of(haystack, "test", false).empty());
    }

    SECTION("With substring included") {
        REQUIRE(rdk::from_last_occurrence_of(haystack, "test", true) == "test");
    }

    SECTION("With invalid substring") {
        REQUIRE(rdk::from_first_occurrence_of(haystack, "bleep", true).empty());
    }
}

TEST_CASE("Test stringContains", "[StringUtilities]") {
    SECTION("Without substring included") {
        REQUIRE(rdk::string_contains("ABC", 'C'));
        REQUIRE_FALSE(rdk::string_contains("ABC", 'c'));
        REQUIRE(rdk::string_contains("ABC", 'B'));
    }
}

TEST_CASE("Test fromStringStrict", "[StringUtilities]") {
    SECTION("An integer should be successfully parsed") {
        auto result = rdk::from_string_strict<int>("1");
        REQUIRE(result.has_value());
        REQUIRE(*result == 1);
    }

    SECTION("A string with non valid characters should not return a result") {
        auto result = rdk::from_string_strict<int>("1 ");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("A string with non valid characters should not return a result") {
        auto result = rdk::from_string_strict<int>(" 1 ");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("A string with non valid characters should not return a result") {
        auto result = rdk::from_string_strict<int>(" 1");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("A string with non valid characters should not return a result") {
        auto result = rdk::from_string_strict<int>("1A");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("A maximum value should be returned") {
        auto result = rdk::from_string_strict<int>(std::to_string(std::numeric_limits<int>::max()));
        REQUIRE(result.has_value());
        REQUIRE(*result == std::numeric_limits<int>::max());
    }

    SECTION("A too big value should return no value") {
        auto result = rdk::from_string_strict<int>(std::to_string(std::numeric_limits<int>::max()) + "1");
        REQUIRE_FALSE(result.has_value());
    }
}

TEST_CASE("Test fromString", "[StringUtilities]") {
    SECTION("An integer should be successfully parsed") {
        auto result = rdk::from_string<int>("1");
        REQUIRE(result.has_value());
        REQUIRE(*result == 1);
    }

    SECTION("A string with non valid characters should still return a result") {
        auto result = rdk::from_string<int>("1 ");
        REQUIRE(result.has_value());
        REQUIRE(*result == 1);
    }

    SECTION("A string with non valid characters should still return a result") {
        auto result = rdk::from_string<int>("1A");
        REQUIRE(result.has_value());
        REQUIRE(*result == 1);
    }

    SECTION("A string starting with non valid characters should not return a result") {
        auto result = rdk::from_string<int>(" 1 ");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("A string starting with non valid characters should not return a result") {
        auto result = rdk::from_string<int>(" 1");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("A maximum value should be returned") {
        auto result = rdk::from_string<int>(std::to_string(std::numeric_limits<int>::max()));
        REQUIRE(result.has_value());
        REQUIRE(*result == std::numeric_limits<int>::max());
    }

    SECTION("A too big value should return no value") {
        auto result = rdk::from_string<int>(std::to_string(std::numeric_limits<int>::max()) + "1");
        REQUIRE_FALSE(result.has_value());
    }
}

TEST_CASE("Test countNumberOfEqualCharactersFromStart", "[StringUtilities]") {
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"test 1", "test 2", "test 3"}) == 5);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"test1", "test2", "test3"}) == 4);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"tes1", "tes2", "tes3"}) == 3);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"te1", "te2", "te3"}) == 2);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"t1", "t2", "t3"}) == 1);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"1", "2", "3"}) == 0);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"1", "2", ""}) == 0);
    REQUIRE(rdk::count_number_of_equal_characters_from_start({"", "", ""}) == 0);
}

TEST_CASE("Test mergeStrings", "[StringUtilities]") {
    SECTION("Merging 2 strings which differ ate the end should merge them") {
        REQUIRE(rdk::merge_strings({"test 1", "test 2", "test 3"}, " & ") == "test 1 & 2 & 3");
        REQUIRE(rdk::merge_strings({"test 1", "test 2", "test"}, " & ") == "test 1 & 2");
        REQUIRE(rdk::merge_strings({"test 1", "test 2"}, "|") == "test 1|2");
        REQUIRE(rdk::merge_strings({"test1", "test2", "test3"}, " & ") == "test1 & 2 & 3");
        REQUIRE(rdk::merge_strings({"Computer 11", "Computer 12"}, " & ") == "Computer 11 & 12");
    }

    SECTION("Totally different string should be concatenated") {
        REQUIRE(rdk::merge_strings({"one 1", "two 2", "three 3"}, " & ") == "one 1 & two 2 & three 3");
        REQUIRE(rdk::merge_strings({"equal", "equal", "three 3"}, " & ") == "equal & equal & three 3");
    }
}

TEST_CASE("Test removePrefix", "[StringUtilities]") {
    SECTION("Remove valid prefix") {
        std::string_view str = "some/random/string";
        REQUIRE(rdk::remove_prefix(str, "some/"));
        REQUIRE(str == "random/string");
    }

    SECTION("Remove invalid prefix") {
        std::string_view str = "some/random/string";
        REQUIRE_FALSE(rdk::remove_prefix(str, "noexist/"));
        REQUIRE(str == "some/random/string");
    }

    SECTION("Remove invalid prefix") {
        std::string_view str = "test/some/random/string";
        REQUIRE_FALSE(rdk::remove_prefix(str, "some/"));
        REQUIRE(str == "test/some/random/string");
    }
}

TEST_CASE("Test removeSuffix", "[StringUtilities]") {
    SECTION("Remove valid suffix") {
        std::string_view str = "some/random/string";
        REQUIRE(rdk::remove_suffix(str, "/string"));
        REQUIRE(str == "some/random");
    }

    SECTION("Remove invalid suffix") {
        std::string_view str = "some/random/string";
        REQUIRE_FALSE(rdk::remove_suffix(str, "/noexist"));
        REQUIRE(str == "some/random/string");
    }

    SECTION("Remove invalid suffix") {
        std::string_view str = "some/random/string/test";
        REQUIRE_FALSE(rdk::remove_suffix(str, "/string"));
        REQUIRE(str == "some/random/string/test");
    }
}
