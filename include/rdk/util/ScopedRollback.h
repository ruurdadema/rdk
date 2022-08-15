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
 * This class gathers one or more functions and executes those on destruction, if no call to commit was done before.
 * This class acts as alternative to the 'goto cleanup' paradigm from C, where we want to rollback previous changes if a
 * following change did not succeed. This class is also exception safe.
 */
class ScopedRollback
{
public:
    ScopedRollback() = default;

    /**
     * Constuctor.
     * @param initialFunctionToRollback Function to execute on rollback.
     */
    explicit ScopedRollback (std::function<void()>&& initialFunctionToRollback)
    {
        mRollbackFunctions.push_back (std::move (initialFunctionToRollback));
    }

    /**
     * Executes all functions left in the internal array.
     */
    ~ScopedRollback()
    {
        for (auto& func : mRollbackFunctions)
            if (func)
                func();
    }

    ScopedRollback (const ScopedRollback&) = delete;
    ScopedRollback& operator= (const ScopedRollback&) = delete;

    /**
     * Adds a function to rollback.
     * @param functionToRollback Function to rollback, if not committed.
     */
    void add (std::function<void()>&& functionToRollback)
    {
        mRollbackFunctions.push_back (std::move (functionToRollback));
    }

    /**
     * Commits all the functions, meaning it will clear all currently stored functions. You'll want to call this
     * whenever all operations were successful and nothing need to be rolled back.
     */
    void commit()
    {
        mRollbackFunctions.clear();
    }

private:
    std::vector<std::function<void()>> mRollbackFunctions;
};

} // namespace rdk
