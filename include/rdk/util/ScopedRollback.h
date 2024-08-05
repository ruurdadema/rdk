//
// Created by Ruurd Adema on 15/08/2022.
// Copyright (c) 2021 Owllab. All rights reserved.
//

#pragma once

#include <functional>
#include <vector>

namespace rdk
{

/**
 * This class gathers one or more functions and executes those on destruction if no call to cancel was done before.
 * This class acts as alternative to the 'goto cleanup' paradigm from C, where we want to rollback previous changes if a
 * following change did not succeed. This class is also exception safe.
 */
class ScopedRollback
{
public:
    ScopedRollback() = default;

    /**
     * Constructor.
     * @param initial_rollback_function Function to execute on rollback.
     */
    explicit ScopedRollback (std::function<void()>&& initial_rollback_function)
    {
        rollback_functions_.push_back (std::move (initial_rollback_function));
    }

    /**
     * Executes all functions left in the internal array.
     */
    ~ScopedRollback()
    {
        for (auto& func : rollback_functions_) {
            if (func) {
                func();
            }
        }
    }

    ScopedRollback (const ScopedRollback&) = delete;
    ScopedRollback& operator= (const ScopedRollback&) = delete;

    /**
     * Adds a function to rollback.
     * @param functionToRollback Function to rollback, if not committed.
     */
    void add (std::function<void()>&& functionToRollback)
    {
        rollback_functions_.push_back (std::move (functionToRollback));
    }

    /**
     * Cancels all the functions, meaning it will clear all currently stored functions. You'll want to call this
     * whenever all operations were successful and nothing need to be rolled back.
     */
    void cancel()
    {
        rollback_functions_.clear();
    }

private:
    std::vector<std::function<void()>> rollback_functions_;
};

} // namespace rdk
