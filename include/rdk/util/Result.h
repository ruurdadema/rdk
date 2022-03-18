//
// Created by Ruurd Adema on 14/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <string>
#include <utility>

namespace rdk
{

/**
 * Utility class for indicating success, or an error with accompanying error message.
 */
class Result
{
public:
    /**
     * Returns an ok result;
     */
    static Result ok() { return {}; }

    /**
     * Returns an error with given message.
     * @param errorMessage The error message to assign. When empty, the the message will be "Unknown Error".
     * @return The Result containing the error message.
     */
    static Result error (const std::string& errorMessage)
    {
        return Result (errorMessage.empty() ? "Unknown Error" : errorMessage);
    }

    /**
     * Returns true if this Result indicates success, or false otherwise.
     */
    bool isOk() { return mErrorMessage.empty(); }

    /**
     * Returns true if this Result contains a non-empty error message, or false otherwise.
     */
    bool hasError() { return !mErrorMessage.empty(); }

    /**
     * Returns true if this Result indicates success, or false otherwise. Same as isOk().
     */
    explicit operator bool() const noexcept { return mErrorMessage.empty(); }

    /**
     * Returns true if this Result contains a non-empty error message, or false otherwise. Same as hasError().
     */
    bool operator!() const noexcept { return !mErrorMessage.empty(); }

    /**
     * Returns the contained error message, which will be empty if this Result indicates success.
     */
    [[nodiscard]] const std::string& getErrorMessage() const { return mErrorMessage; }

private:
    std::string mErrorMessage;

    Result() noexcept = default;
    explicit Result (std::string value) noexcept : mErrorMessage (std::move (value)) {}
};

/**
 * Small convenience method to allow writing rdk::ok() instead of rdk::Result::ok().
 * It's for the lazy people like myself.
 * @return Result indicating failure or success.
 */
inline Result ok()
{
    return Result::ok();
}

/**
 * Small convenience method for allowing writing rdk::error() instead of rdk::Result::error().
 * It's for the lazy people like myself.
 * @param errorMessage Message describing the error.
 * @return Result indicating failure or success.
 */
inline Result error (const std::string& errorMessage)
{
    return Result::error (errorMessage);
}

} // namespace rdk
