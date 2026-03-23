//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#include <rdk/string/StringUtilities.h>

#include <algorithm>
#include <limits>
#include <string>

extern "C" {
#include "../lib/natsort/strnatcmp.h"
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