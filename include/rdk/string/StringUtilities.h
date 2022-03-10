//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <charconv>
#include <optional>
#include <string_view>
#include <vector>

namespace rdk
{

/**
 * Returns a string truncated up to the first occurrence of a needle.
 * @param stringToSearchIn String to search in.
 * @param stringToSearchFor String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view upToFirstOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult);

/**
 * Returns a string truncated up to the nth occurrence of a needle.
 * @param nth Nth needle to find, 0 will always return an empty string, 1 will find the 1st needle, and so on.
 * @param stringToSearchIn String to search in.
 * @param stringToSearchFor String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view upToTheNthOccurrenceOf (
    size_t nth,
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult);

/**
 * Returns a string truncated up to the last occurrence of a needle.
 * @param stringToSearchIn String to search in.
 * @param stringToSearchFor String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view upToLastOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult);

/**
 * Returns a string truncated starting from to first last occurrence of a needle.
 * @param stringToSearchIn String to search in.
 * @param stringToSearchFor String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view fromFirstOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult);

/**
 * Returns a string truncated from the nth occurrence of a needle.
 * @param nth Nth needle to find, 0 will always return the full string, 1 will find the 1st needle, and so on.
 * @param stringToSearchIn String to search in.
 * @param stringToSearchFor String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view fromNthOccurrenceOf (
    size_t nth,
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult);

/**
 * Returns a string truncated starting from to last last occurrence of a needle.
 * @param stringToSearchIn String to search in.
 * @param stringToSearchFor String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view fromLastOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult);

/**
 * Removes given prefix from given string_view, if prefix is found at the beginning of string.
 * @param string String to remove prefix from.
 * @param prefixToRemove Prefix to find and remove.
 * @return True if prefix was removed, or false if prefix was not found and string was not altered.
 */
bool removePrefix (std::string_view& string, std::string_view prefixToRemove);

/**
 * Removes given suffix from given string_view, if suffix is found at the end of string.
 * @param string String to remove suffix from.
 * @param suffixToRemove Suffix to find and remove.
 * @return True if suffix was removed, or false if suffix was not found and string was not altered.
 */
bool removeSuffix (std::string_view& string, std::string_view suffixToRemove);

/**
 * Small convenience function around std::from_chars.
 * @tparam Type Type of the value to convert from a string.
 * @param string String to convert to a value.
 * @return The converted value as optional, which will contain a value on success or will be empty on failure.
 */
template <typename Type> std::optional<Type> fromString (std::string_view string)
{
    Type result {};
    auto [p, ec] = std::from_chars (string.data(), string.data() + string.size(), result);
    if (ec == std::errc())
        return result;
    return {};
}

/**
 * Small convenience function around std::from_chars.
 * It will only return a valid result if the whole string was a number.
 * @tparam Type Type of the value to convert from a string.
 * @param string String to convert to a value.
 * @return The converted value as optional, which will contain a value on success or will be empty on failure.
 */
template <typename Type> std::optional<Type> fromStringStrict (std::string_view string)
{
    Type result {};
    auto [p, ec] = std::from_chars (string.data(), string.data() + string.size(), result);
    if (ec == std::errc() && p >= string.data() + string.size())
        return result;
    return {};
}

/**
 * Returns whether given string contains a certain character.
 * @param string String to look into.
 * @param c Character to find.
 * @return True if character was found, of false if character was not found.
 */
bool stringContains (std::string_view string, char c);

/**
 * Creates a vector containing the contents of the given string view.
 * @param string String to put inside the vector.
 * @return Vector containing the string.
 */
static inline std::vector<uint8_t> toVector (std::string_view string)
{
    return { string.begin(), string.end() };
}

/**
 * Compares 2 string while taking numbers into account.
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 * @param caseSensitive Whether the comparison should be case sensitive.
 * @return Comparison result.
 */
int compareNatural (const std::string& lhs, const std::string& rhs, bool caseSensitive = true);

/**
 * Can be used with std::sort to sort a vector of strings alphabetically and naturally.
 */
struct NumericAwareSortFunctor
{
    inline bool operator() (const std::string& lhs, const std::string& rhs)
    {
        return compareNatural (lhs, rhs, false) < 0;
    }
};

size_t countNumberOfEqualCharactersFromStart (const std::vector<std::string>& strings);

std::string mergeStrings (const std::vector<std::string>& strings, const char* coupleCharacters = " & ");

} // namespace rdk
