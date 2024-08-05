//
// Created by Ruurd Adema on 14/12/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <string>

namespace rdk {

/**
 * Utility class for indicating success, or an error with accompanying error message.
 */
class Result {
  public:
    /**
     * Returns an ok result;
     */
    static Result ok() {
        return {};
    }

    /**
     * Returns an error with given message.
     * @param error_message The error message to assign. When empty, the the message will be "Unknown Error".
     * @return The Result containing the error message.
     */
    static Result error(const std::string& error_message) {
        return Result(error_message.empty() ? "Unknown Error" : error_message);
    }

    /**
     * Returns true if this Result indicates success, or false otherwise.
     */
    [[nodiscard]] bool is_ok() const {
        return error_message_.empty();
    }

    /**
     * Returns true if this Result contains a non-empty error message, or false otherwise.
     */
    [[nodiscard]] bool has_error() const {
        return !error_message_.empty();
    }

    /**
     * Returns true if this Result indicates success, or false otherwise. Same as isOk().
     */
    explicit operator bool() const noexcept {
        return error_message_.empty();
    }

    /**
     * Returns true if this Result contains a non-empty error message, or false otherwise. Same as hasError().
     */
    bool operator!() const noexcept {
        return !error_message_.empty();
    }

    /**
     * Returns the contained error message, which will be empty if this Result indicates success.
     */
    [[nodiscard]] const std::string& get_error_message() const {
        return error_message_;
    }

  private:
    std::string error_message_;

    Result() noexcept = default;

    explicit Result(std::string value) noexcept : error_message_(std::move(value)) {}
};

/**
 * Small convenience method to allow writing rdk::ok() instead of rdk::Result::ok().
 * It's for the lazy people like myself.
 * @return Result indicating failure or success.
 */
inline Result ok() {
    return Result::ok();
}

/**
 * Small convenience method for allowing writing rdk::error() instead of rdk::Result::error().
 * It's for the lazy people like myself.
 * @param error_message Message describing the error.
 * @return Result indicating failure or success.
 */
inline Result error(const std::string& error_message) {
    return Result::error(error_message);
}

}  // namespace rdk
