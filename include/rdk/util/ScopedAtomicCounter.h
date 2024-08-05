//
// Created by Ruurd Adema on 12/08/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <atomic>

template<class Type>
class ScopedAtomicCounter {
  public:
    ScopedAtomicCounter(const ScopedAtomicCounter& other) : atomic_(other.atomic_) {
        previous_value_ = atomic_.fetch_add(1);
    }

    ScopedAtomicCounter(ScopedAtomicCounter&& other) noexcept = delete;

    explicit ScopedAtomicCounter(std::atomic<Type>& atomic) : atomic_(atomic) {
        previous_value_ = atomic_.fetch_add(1);
    }

    ScopedAtomicCounter& operator=(const ScopedAtomicCounter& other) {
        atomic_ = other.atomic_;
        previous_value_ = atomic_.fetch_add(1);
        return *this;
    }

    ScopedAtomicCounter& operator=(ScopedAtomicCounter&&) noexcept = delete;

    ~ScopedAtomicCounter() {
        atomic_.fetch_add(-1);
    }

    [[nodiscard]] Type previous_value() const {
        return previous_value_;
    }

  private:
    std::atomic<Type>& atomic_;
    Type previous_value_ {};
};
