//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#include "rdk/util/StringUtilities.h"

#include <algorithm>
#include <limits>
#include <string>

extern "C" {
#include "../../lib/natsort/strnatcmp.h"
}

bool rdk::starts_with(const std::string_view text, const std::string_view start) {
    return text.rfind(start, 0) == 0;
}

std::string_view rdk::up_to_first_occurrence_of(
    std::string_view string_to_search_in,
    const std::string_view string_to_search_for,
    const bool include_sub_string_in_result
) {
    const auto pos = string_to_search_in.find(string_to_search_for);

    if (pos == std::string_view::npos)
        return {};

    return string_to_search_in.substr(0, include_sub_string_in_result ? pos + string_to_search_for.size() : pos);
}

std::string_view rdk::up_to_the_nth_occurrence_of(
    const size_t nth,
    std::string_view string_to_search_in,
    const std::string_view string_to_search_for,
    const bool include_sub_string_in_result
) {
    size_t pos = std::string_view::npos;

    for (size_t i = 0; i < nth; ++i) {
        pos = string_to_search_in.find(string_to_search_for, pos + 1);

        if (pos == std::string_view::npos) {
            return {};
        }
    }

    if (pos == std::string_view::npos) {
        return {};
    }

    return string_to_search_in.substr(0, include_sub_string_in_result ? pos + string_to_search_for.size() : pos);
}

std::string_view rdk::up_to_last_occurrence_of(
    const std::string_view string_to_search_in,
    const std::string_view string_to_search_for,
    const bool include_sub_string_in_result
) {
    const auto pos = string_to_search_in.rfind(string_to_search_for);

    if (pos == std::string_view::npos)
        return {};

    return string_to_search_in.substr(0, include_sub_string_in_result ? pos + string_to_search_for.size() : pos);
}

std::string_view rdk::from_first_occurrence_of(
    std::string_view string_to_search_in,
    const std::string_view string_to_search_for,
    const bool include_sub_string_in_result
) {
    const auto pos = string_to_search_in.find(string_to_search_for);

    if (pos == std::string_view::npos)
        return {};

    return string_to_search_in.substr(include_sub_string_in_result ? pos : pos + string_to_search_for.size());
}

std::string_view rdk::from_last_occurrence_of(
    const std::string_view string_to_search_in,
    const std::string_view string_to_search_for,
    const bool includeSubStringInResult
) {
    const auto pos = string_to_search_in.rfind(string_to_search_for);

    if (pos == std::string_view::npos) {
        return {};
    }

    return string_to_search_in.substr(includeSubStringInResult ? pos : pos + string_to_search_for.size());
}

bool rdk::string_contains(const std::string_view string, char c) {
    return std::any_of(string.begin(), string.end(), [c](const char& item) { return item == c; });
}

std::string_view rdk::from_nth_occurrence_of(
    const size_t nth,
    std::string_view string_to_search_in,
    const std::string_view string_to_search_for,
    const bool includeSubStringInResult
) {
    size_t pos = std::string_view::npos;

    for (size_t i = 0; i < nth; ++i) {
        pos = string_to_search_in.find(string_to_search_for, pos + 1);

        if (pos == std::string_view::npos) {
            return {};
        }
    }

    if (pos == std::string_view::npos) {
        return {};
    }

    return string_to_search_in.substr(
        includeSubStringInResult ? pos : pos + string_to_search_for.size(),
        string_to_search_in.size()
    );
}

int rdk::compare_natural(const std::string& lhs, const std::string& rhs, const bool case_sensitive) {
    return case_sensitive ? strnatcmp(lhs.c_str(), rhs.c_str()) : strnatcasecmp(lhs.c_str(), rhs.c_str());
}

size_t rdk::count_number_of_equal_characters_from_start(const std::vector<std::string>& strings) {
    if (strings.size() <= 1)
        return 0;

    const std::string& base = strings.front();

    size_t highestMatchingIndex = std::numeric_limits<size_t>::max();

    // Iterate strings, except the first one
    for (size_t i = 1; i < strings.size(); ++i) {
        // Iterate characters.
        for (size_t ch = 0; ch < base.size() && ch < strings[i].size(); ++ch) {
            if (strings[i][ch] != base[ch]) {
                highestMatchingIndex = std::min(highestMatchingIndex, ch);
                break;
            }
        }
    }

    if (highestMatchingIndex == std::numeric_limits<size_t>::max())
        return 0;

    return highestMatchingIndex;
}

std::string rdk::merge_strings(const std::vector<std::string>& strings, const char* couple_characters) {
    if (strings.empty())
        return {};

    auto count = count_number_of_equal_characters_from_start(strings);
    if (count == 0) {
        // Just concatenate all strings
        std::string output = strings.front();
        for (size_t i = 1; i < strings.size(); ++i) {
            output.append(couple_characters);
            output.append(strings[i]);
        }
        return output;
    }

    std::string output = strings.front();

    // If the equal part contains a space, limit the amount of equal characters to that.
    for (size_t i = 0; i < count; ++i) {
        if (output[i] == ' ') {
            count = i + 1;
            break;
        }
    }

    for (size_t i = 1; i < strings.size(); ++i) {
        if (strings[i].size() > count) {
            output.append(couple_characters);
            output.append(strings[i].substr(count));
        }
    }

    return output;
}

bool rdk::remove_prefix(std::string_view& string, const std::string_view prefix_to_remove) {
    const auto pos = string.find(prefix_to_remove);

    if (pos != 0)  // If prefix doesn't start at the beginning.
        return false;

    string = string.substr(pos + prefix_to_remove.size());
    return true;
}

bool rdk::remove_suffix(std::string_view& string, const std::string_view suffix_to_remove) {
    const auto pos = string.rfind(suffix_to_remove);

    if (pos != string.size() - suffix_to_remove.size()) {
        return false;
    }

    string = string.substr(0, pos);

    return true;
}
