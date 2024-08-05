//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <charconv>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace rdk {

/**
 * Tests whether given text starts with a certain string.
 * @param text The text to test.
 * @param start
 * @return
 */
bool starts_with(std::string_view text, std::string_view start);

/**
 * Returns a string truncated up to the first occurrence of a needle.
 * @param string_to_search_in String to search in.
 * @param string_to_search_for String to search for.
 * @param include_sub_string_in_result If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view up_to_first_occurrence_of(
    std::string_view string_to_search_in,
    std::string_view string_to_search_for,
    bool include_sub_string_in_result
);

/**
 * Returns a string truncated up to the nth occurrence of a needle.
 * @param nth Nth needle to find, 0 will always return an empty string, 1 will find the 1st needle, and so on.
 * @param string_to_search_in String to search in.
 * @param string_to_search_for String to search for.
 * @param include_sub_string_in_result If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view up_to_the_nth_occurrence_of(
    size_t nth,
    std::string_view string_to_search_in,
    std::string_view string_to_search_for,
    bool include_sub_string_in_result
);

/**
 * Returns a string truncated up to the last occurrence of a needle.
 * @param string_to_search_in String to search in.
 * @param string_to_search_for String to search for.
 * @param include_sub_string_in_result If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view up_to_last_occurrence_of(
    std::string_view string_to_search_in,
    std::string_view string_to_search_for,
    bool include_sub_string_in_result
);

/**
 * Returns a string truncated starting from to first last occurrence of a needle.
 * @param string_to_search_in String to search in.
 * @param string_to_search_for String to search for.
 * @param include_sub_string_in_result If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view from_first_occurrence_of(
    std::string_view string_to_search_in,
    std::string_view string_to_search_for,
    bool include_sub_string_in_result
);

/**
 * Returns a string truncated from the nth occurrence of a needle.
 * @param nth Nth needle to find, 0 will always return the full string, 1 will find the 1st needle, and so on.
 * @param string_to_search_in String to search in.
 * @param string_to_search_for String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view from_nth_occurrence_of(
    size_t nth,
    std::string_view string_to_search_in,
    std::string_view string_to_search_for,
    bool includeSubStringInResult
);

/**
 * Returns a string truncated starting from to last last occurrence of a needle.
 * @param string_to_search_in String to search in.
 * @param string_to_search_for String to search for.
 * @param includeSubStringInResult If true the needle will be included in the resulting string, when false it will not.
 * @return The truncated string, or an empty string when no needle was found.
 */
std::string_view from_last_occurrence_of(
    std::string_view string_to_search_in,
    std::string_view string_to_search_for,
    bool includeSubStringInResult
);

/**
 * Removes given prefix from given string_view, if prefix is found at the beginning of string.
 * @param string String to remove prefix from.
 * @param prefix_to_remove Prefix to find and remove.
 * @return True if prefix was removed, or false if prefix was not found and string was not altered.
 */
bool remove_prefix(std::string_view& string, std::string_view prefix_to_remove);

/**
 * Removes given suffix from given string_view, if suffix is found at the end of string.
 * @param string String to remove suffix from.
 * @param suffix_to_remove Suffix to find and remove.
 * @return True if suffix was removed, or false if suffix was not found and string was not altered.
 */
bool remove_suffix(std::string_view& string, std::string_view suffix_to_remove);

/**
 * Small convenience function around std::from_chars.
 * @tparam Type Type of the value to convert from a string.
 * @param string String to convert to a value.
 * @return The converted value as optional, which will contain a value on success or will be empty on failure.
 */
template<typename Type>
std::optional<Type> from_string(std::string_view string) {
    Type result {};
    auto [p, ec] = std::from_chars(string.data(), string.data() + string.size(), result);
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
template<typename Type>
std::optional<Type> from_string_strict(std::string_view string) {
    Type result {};
    auto [p, ec] = std::from_chars(string.data(), string.data() + string.size(), result);
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
bool string_contains(std::string_view string, char c);

/**
 * Creates a vector containing the contents of the given string view.
 * @param string String to put inside the vector.
 * @return Vector containing the string.
 */
[[maybe_unused]] static std::vector<uint8_t> to_vector(std::string_view string) {
    return {string.begin(), string.end()};
}

/**
 * Compares 2 string while taking numbers into account.
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 * @param case_sensitive Whether the comparison should be case sensitive.
 * @return Comparison result.
 */
int compare_natural(const std::string& lhs, const std::string& rhs, bool case_sensitive = true);

/**
 * Can be used with std::sort to sort a vector of strings alphabetically and naturally.
 */
struct NumericAwareSortFunctor {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
        return compare_natural(lhs, rhs, false) < 0;
    }
};

size_t count_number_of_equal_characters_from_start(const std::vector<std::string>& strings);

std::string merge_strings(const std::vector<std::string>& strings, const char* couple_characters = " & ");

}  // namespace rdk
