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
#include <string>

namespace rdk {

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

}  // namespace rdk
