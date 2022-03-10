//
// Created by Ruurd Adema on 12/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

template <class Type> class ScopedAtomicCounter
{
public:
    ScopedAtomicCounter (const ScopedAtomicCounter& other) : mAtomic (other.mAtomic)
    {
        mPreviousValue = mAtomic.fetch_add (1);
    }

    ScopedAtomicCounter (ScopedAtomicCounter&& other) noexcept = delete;

    explicit ScopedAtomicCounter (std::atomic<Type>& atomic) : mAtomic (atomic)
    {
        mPreviousValue = mAtomic.fetch_add (1);
    }

    ScopedAtomicCounter& operator= (const ScopedAtomicCounter& other)
    {
        mAtomic = other.mAtomic;
        mPreviousValue = mAtomic.fetch_add (1);
    }

    ScopedAtomicCounter& operator= (ScopedAtomicCounter&&) noexcept = delete;

    ~ScopedAtomicCounter() { mAtomic.fetch_add (-1); }

    [[nodiscard]] Type previousValue() const { return mPreviousValue; }

private:
    std::atomic<Type>& mAtomic;
    Type mPreviousValue {};
};
