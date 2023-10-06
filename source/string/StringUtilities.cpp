//
// Created by Ruurd Adema on 10/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#include <algorithm>
#include <rdk/string/StringUtilities.h>
#include <string>
#include <limits>

extern "C"
{
#include "../lib/natsort/strnatcmp.h"
}

bool rdk::startsWith (std::string_view text, std::string_view start)
{
    return text.rfind (start, 0) == 0;
}

std::string_view rdk::upToFirstOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult)
{
    auto pos = stringToSearchIn.find (stringToSearchFor);

    if (pos == std::string_view::npos)
        return {};

    return stringToSearchIn.substr (0, includeSubStringInResult ? pos + stringToSearchFor.size() : pos);
}

std::string_view rdk::upToTheNthOccurrenceOf (
    size_t nth,
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult)
{
    size_t pos = std::string_view::npos;
    for (size_t i = 0; i < nth; ++i)
    {
        pos = stringToSearchIn.find (stringToSearchFor, pos + 1);

        if (pos == std::string_view::npos)
            return {};
    }

    if (pos == std::string_view::npos)
        return {};

    return stringToSearchIn.substr (0, includeSubStringInResult ? pos + stringToSearchFor.size() : pos);
}

std::string_view rdk::upToLastOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult)
{
    auto pos = stringToSearchIn.rfind (stringToSearchFor);

    if (pos == std::string_view::npos)
        return {};

    return stringToSearchIn.substr (0, includeSubStringInResult ? pos + stringToSearchFor.size() : pos);
}

std::string_view rdk::fromFirstOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult)
{
    auto pos = stringToSearchIn.find (stringToSearchFor);

    if (pos == std::string_view::npos)
        return {};

    return stringToSearchIn.substr (includeSubStringInResult ? pos : pos + stringToSearchFor.size());
}

std::string_view rdk::fromLastOccurrenceOf (
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult)
{
    auto pos = stringToSearchIn.rfind (stringToSearchFor);

    if (pos == std::string_view::npos)
        return {};

    return stringToSearchIn.substr (includeSubStringInResult ? pos : pos + stringToSearchFor.size());
}

bool rdk::stringContains (std::string_view string, char c)
{
    return std::any_of (string.begin(), string.end(), [c] (const char& item) {
        return item == c;
    });
}

std::string_view rdk::fromNthOccurrenceOf (
    size_t nth,
    std::string_view stringToSearchIn,
    std::string_view stringToSearchFor,
    bool includeSubStringInResult)
{
    size_t pos = std::string_view::npos;
    for (size_t i = 0; i < nth; ++i)
    {
        pos = stringToSearchIn.find (stringToSearchFor, pos + 1);

        if (pos == std::string_view::npos)
            return {};
    }

    if (pos == std::string_view::npos)
        return {};

    return stringToSearchIn.substr (
        includeSubStringInResult ? pos : pos + stringToSearchFor.size(),
        stringToSearchIn.size());
}

int rdk::compareNatural (const std::string& lhs, const std::string& rhs, bool caseSensitive)
{
    return caseSensitive ? strnatcmp (lhs.c_str(), rhs.c_str()) : strnatcasecmp (lhs.c_str(), rhs.c_str());
}

size_t rdk::countNumberOfEqualCharactersFromStart (const std::vector<std::string>& strings)
{
    if (strings.size() <= 1)
        return 0;

    const std::string& base = strings.front();

    size_t highestMatchingIndex = std::numeric_limits<size_t>::max();

    // Iterate strings, except the first one
    for (size_t i = 1; i < strings.size(); ++i)
    {
        // Iterate characters.
        for (size_t ch = 0; ch < base.size() && ch < strings[i].size(); ++ch)
        {
            if (strings[i][ch] != base[ch])
            {
                highestMatchingIndex = std::min (highestMatchingIndex, ch);
                break;
            }
        }
    }

    if (highestMatchingIndex == std::numeric_limits<size_t>::max())
        return 0;

    return highestMatchingIndex;
}

std::string rdk::mergeStrings (const std::vector<std::string>& strings, const char* coupleCharacters)
{
    if (strings.empty())
        return {};

    auto count = countNumberOfEqualCharactersFromStart (strings);
    if (count == 0)
    {
        // Just concatenate all strings
        std::string output = strings.front();
        for (size_t i = 1; i < strings.size(); ++i)
        {
            output.append (coupleCharacters);
            output.append (strings[i]);
        }
        return output;
    }

    std::string output = strings.front();

    // If the equal part contains a space, limit the amount of equal characters to that.
    for (size_t i = 0; i < count; ++i)
    {
        if (output[i] == ' ')
        {
            count = i + 1;
            break;
        }
    }

    for (size_t i = 1; i < strings.size(); ++i)
    {
        if (strings[i].size() > count)
        {
            output.append (coupleCharacters);
            output.append (strings[i].substr (count));
        }
    }

    return output;
}

bool rdk::removePrefix (std::string_view& string, std::string_view prefixToRemove)
{
    auto pos = string.find (prefixToRemove);

    if (pos != 0) // If prefix doesn't start at the beginning.
        return false;

    string = string.substr (pos + prefixToRemove.size());
    return true;
}

bool rdk::removeSuffix (std::string_view& string, std::string_view suffixToRemove)
{
    auto pos = string.rfind (suffixToRemove);

    if (pos != string.size() - suffixToRemove.size())
        return false;

    string = string.substr (0, pos);

    return true;
}
