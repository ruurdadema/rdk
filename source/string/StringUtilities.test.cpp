//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#include <catch2/catch.hpp>

#include <rdk/string/StringUtilities.h>

TEST_CASE ("Test upToFirstOccurrenceOf", "[StringUtilities]")
{
    std::string_view haystack ("one test two test three test");

    SECTION ("Without substring included") { REQUIRE (rdk::upToFirstOccurrenceOf (haystack, "test", false) == "one "); }
    SECTION ("With substring included") { REQUIRE (rdk::upToFirstOccurrenceOf (haystack, "test", true) == "one test"); }
    SECTION ("With invalid substring") { REQUIRE (rdk::upToFirstOccurrenceOf (haystack, "bleep", true).empty()); }
}

TEST_CASE ("Test upToFirstNthOccurrenceOf", "[StringUtilities]")
{
    std::string_view haystack ("/one/two/three/four");

    SECTION ("First occurrence")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (1, haystack, "/", false).empty());
        REQUIRE (rdk::upToTheNthOccurrenceOf (1, haystack, "/", true) == "/");
    }

    SECTION ("Second occurrence")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (2, haystack, "/", false) == "/one");
        REQUIRE (rdk::upToTheNthOccurrenceOf (2, haystack, "/", true) == "/one/");
    }

    SECTION ("Third occurrence")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (3, haystack, "/", false) == "/one/two");
        REQUIRE (rdk::upToTheNthOccurrenceOf (3, haystack, "/", true) == "/one/two/");
    }

    SECTION ("Fourth occurrence")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (4, haystack, "/", false) == "/one/two/three");
        REQUIRE (rdk::upToTheNthOccurrenceOf (4, haystack, "/", true) == "/one/two/three/");
    }

    SECTION ("Multi character needle")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (4, haystack, "two", false).empty());
        REQUIRE (rdk::upToTheNthOccurrenceOf (4, haystack, "two", true).empty());
    }

    SECTION ("Empty string")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (1, "", "/", false).empty());
        REQUIRE (rdk::upToTheNthOccurrenceOf (1, "", "/", true).empty());
    }

    SECTION ("Non-empty string with no needle")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (1, "onetwothreefour", "/", false).empty());
        REQUIRE (rdk::upToTheNthOccurrenceOf (1, "onetwothreefour", "/", true).empty());
    }

    SECTION ("Non-empty string with not enough needles needle")
    {
        REQUIRE (rdk::upToTheNthOccurrenceOf (2, "/onetwothreefour", "/", false).empty());
        REQUIRE (rdk::upToTheNthOccurrenceOf (2, "/onetwothreefour", "/", true).empty());
    }
}

TEST_CASE ("Test upToLastOccurrenceOf", "[StringUtilities]")
{
    std::string_view haystack ("one test two test three test");

    SECTION ("Without substring included")
    {
        REQUIRE (rdk::upToLastOccurrenceOf (haystack, "test", false) == "one test two test three ");
    }

    SECTION ("With substring included")
    {
        REQUIRE (rdk::upToLastOccurrenceOf (haystack, "test", true) == "one test two test three test");
    }

    SECTION ("With invalid substring") { REQUIRE (rdk::upToFirstOccurrenceOf (haystack, "bleep", true).empty()); }
}

TEST_CASE ("Test fromFirstOccurrenceOf", "[StringUtilities]")
{
    std::string_view haystack ("one test two test three test");

    SECTION ("Without substring included")
    {
        REQUIRE (rdk::fromFirstOccurrenceOf (haystack, "test", false) == " two test three test");
    }
    SECTION ("With substring included")
    {
        REQUIRE (rdk::fromFirstOccurrenceOf (haystack, "test", true) == "test two test three test");
    }

    SECTION ("With invalid substring") { REQUIRE (rdk::fromFirstOccurrenceOf (haystack, "bleep", true).empty()); }
}

TEST_CASE ("Test fromNthOccurrenceOf", "[StringUtilities]")
{
    std::string_view haystack ("/one/two/three/four");

    SECTION ("First occurrence")
    {
        REQUIRE (rdk::fromNthOccurrenceOf (1, haystack, "/", false) == "one/two/three/four");
        REQUIRE (rdk::fromNthOccurrenceOf (1, haystack, "/", true) == "/one/two/three/four");
    }

    SECTION ("Fourth occurrence")
    {
        REQUIRE (rdk::fromNthOccurrenceOf (4, haystack, "/", false) == "four");
        REQUIRE (rdk::fromNthOccurrenceOf (4, haystack, "/", true) == "/four");
    }

    SECTION ("Multi character needle")
    {
        REQUIRE (rdk::fromNthOccurrenceOf (1, haystack, "two", false) == "/three/four");
        REQUIRE (rdk::fromNthOccurrenceOf (1, haystack, "two", true) == "two/three/four");
    }

    SECTION ("Empty string")
    {
        REQUIRE (rdk::fromNthOccurrenceOf (1, "", "/", false).empty());
        REQUIRE (rdk::fromNthOccurrenceOf (1, "", "/", true).empty());
    }

    SECTION ("Non-empty string with no needle")
    {
        REQUIRE (rdk::fromNthOccurrenceOf (1, "onetwothreefour", "/", false).empty());
        REQUIRE (rdk::fromNthOccurrenceOf (1, "onetwothreefour", "/", true).empty());
    }

    SECTION ("Non-empty string with not enough needles needle")
    {
        REQUIRE (rdk::fromNthOccurrenceOf (2, "/onetwothreefour", "/", false).empty());
        REQUIRE (rdk::fromNthOccurrenceOf (2, "/onetwothreefour", "/", true).empty());
    }
}

TEST_CASE ("Test fromLastOccurrenceOf", "[StringUtilities]")
{
    std::string_view haystack ("one test two test three test");

    SECTION ("Without substring included") { REQUIRE (rdk::fromLastOccurrenceOf (haystack, "test", false).empty()); }

    SECTION ("With substring included") { REQUIRE (rdk::fromLastOccurrenceOf (haystack, "test", true) == "test"); }

    SECTION ("With invalid substring") { REQUIRE (rdk::fromFirstOccurrenceOf (haystack, "bleep", true).empty()); }
}

TEST_CASE ("Test stringContains", "[StringUtilities]")
{
    SECTION ("Without substring included")
    {
        REQUIRE (rdk::stringContains ("ABC", 'C'));
        REQUIRE_FALSE (rdk::stringContains ("ABC", 'c'));
        REQUIRE (rdk::stringContains ("ABC", 'B'));
    }
}

TEST_CASE ("Test fromStringStrict", "[StringUtilities]")
{
    SECTION ("An integer should be successfully parsed")
    {
        auto result = rdk::fromStringStrict<int> ("1");
        REQUIRE (result.has_value());
        REQUIRE (*result == 1);
    }

    SECTION ("A string with non valid characters should not return a result")
    {
        auto result = rdk::fromStringStrict<int> ("1 ");
        REQUIRE_FALSE (result.has_value());
    }

    SECTION ("A string with non valid characters should not return a result")
    {
        auto result = rdk::fromStringStrict<int> (" 1 ");
        REQUIRE_FALSE (result.has_value());
    }

    SECTION ("A string with non valid characters should not return a result")
    {
        auto result = rdk::fromStringStrict<int> (" 1");
        REQUIRE_FALSE (result.has_value());
    }

    SECTION ("A string with non valid characters should not return a result")
    {
        auto result = rdk::fromStringStrict<int> ("1A");
        REQUIRE_FALSE (result.has_value());
    }

    SECTION ("A maximum value should be returned")
    {
        auto result = rdk::fromStringStrict<int> (std::to_string (std::numeric_limits<int>::max()));
        REQUIRE (result.has_value());
        REQUIRE (*result == std::numeric_limits<int>::max());
    }

    SECTION ("A too big value should return no value")
    {
        auto result = rdk::fromStringStrict<int> (std::to_string (std::numeric_limits<int>::max()) + "1");
        REQUIRE_FALSE (result.has_value());
    }
}

TEST_CASE ("Test fromString", "[StringUtilities]")
{
    SECTION ("An integer should be successfully parsed")
    {
        auto result = rdk::fromString<int> ("1");
        REQUIRE (result.has_value());
        REQUIRE (*result == 1);
    }

    SECTION ("A string with non valid characters should still return a result")
    {
        auto result = rdk::fromString<int> ("1 ");
        REQUIRE (result.has_value());
        REQUIRE (*result == 1);
    }

    SECTION ("A string with non valid characters should still return a result")
    {
        auto result = rdk::fromString<int> ("1A");
        REQUIRE (result.has_value());
        REQUIRE (*result == 1);
    }

    SECTION ("A string starting with non valid characters should not return a result")
    {
        auto result = rdk::fromString<int> (" 1 ");
        REQUIRE_FALSE (result.has_value());
    }

    SECTION ("A string starting with non valid characters should not return a result")
    {
        auto result = rdk::fromString<int> (" 1");
        REQUIRE_FALSE (result.has_value());
    }

    SECTION ("A maximum value should be returned")
    {
        auto result = rdk::fromString<int> (std::to_string (std::numeric_limits<int>::max()));
        REQUIRE (result.has_value());
        REQUIRE (*result == std::numeric_limits<int>::max());
    }

    SECTION ("A too big value should return no value")
    {
        auto result = rdk::fromString<int> (std::to_string (std::numeric_limits<int>::max()) + "1");
        REQUIRE_FALSE (result.has_value());
    }
}

TEST_CASE ("Test countNumberOfEqualCharactersFromStart", "[StringUtilities]")
{
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "test 1", "test 2", "test 3" }) == 5);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "test1", "test2", "test3" }) == 4);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "tes1", "tes2", "tes3" }) == 3);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "te1", "te2", "te3" }) == 2);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "t1", "t2", "t3" }) == 1);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "1", "2", "3" }) == 0);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "1", "2", "" }) == 0);
    REQUIRE (rdk::countNumberOfEqualCharactersFromStart ({ "", "", "" }) == 0);
}

TEST_CASE ("Test mergeStrings", "[StringUtilities]")
{
    SECTION ("Merging 2 strings which differ ate the end should merge them")
    {
        REQUIRE (rdk::mergeStrings ({ "test 1", "test 2", "test 3" }, " & ") == "test 1 & 2 & 3");
        REQUIRE (rdk::mergeStrings ({ "test 1", "test 2", "test" }, " & ") == "test 1 & 2");
        REQUIRE (rdk::mergeStrings ({ "test 1", "test 2" }, "|") == "test 1|2");
        REQUIRE (rdk::mergeStrings ({ "test1", "test2", "test3" }, " & ") == "test1 & 2 & 3");
        REQUIRE (rdk::mergeStrings ({ "Computer 11", "Computer 12" }, " & ") == "Computer 11 & 12");
    }

    SECTION ("Totally different string should be concatenated")
    {
        REQUIRE (rdk::mergeStrings ({ "one 1", "two 2", "three 3" }, " & ") == "one 1 & two 2 & three 3");
        REQUIRE (rdk::mergeStrings ({ "equal", "equal", "three 3" }, " & ") == "equal & equal & three 3");
    }
}

TEST_CASE ("Test removePrefix", "[StringUtilities]")
{
    SECTION ("Remove valid prefix")
    {
        std::string_view str = "some/random/string";
        REQUIRE (rdk::removePrefix (str, "some/"));
        REQUIRE (str == "random/string");
    }

    SECTION ("Remove invalid prefix")
    {
        std::string_view str = "some/random/string";
        REQUIRE_FALSE (rdk::removePrefix (str, "noexist/"));
        REQUIRE (str == "some/random/string");
    }

    SECTION ("Remove invalid prefix")
    {
        std::string_view str = "test/some/random/string";
        REQUIRE_FALSE (rdk::removePrefix (str, "some/"));
        REQUIRE (str == "test/some/random/string");
    }
}

TEST_CASE ("Test removeSuffix", "[StringUtilities]")
{
    SECTION ("Remove valid suffix")
    {
        std::string_view str = "some/random/string";
        REQUIRE (rdk::removeSuffix (str, "/string"));
        REQUIRE (str == "some/random");
    }

    SECTION ("Remove invalid suffix")
    {
        std::string_view str = "some/random/string";
        REQUIRE_FALSE (rdk::removeSuffix (str, "/noexist"));
        REQUIRE (str == "some/random/string");
    }

    SECTION ("Remove invalid suffix")
    {
        std::string_view str = "some/random/string/test";
        REQUIRE_FALSE (rdk::removeSuffix (str, "/string"));
        REQUIRE (str == "some/random/string/test");
    }
}
