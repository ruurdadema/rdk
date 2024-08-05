//
// Created by Ruurd Adema on 21/01/2022.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <functional>

#include "rdk/detail/NonCopyable.h"

namespace rdk {

/**
 * Class which holds a counter and optionally a function which gets called when the counter drops to 0.
 */
class CountedGuard: NonCopyable {
  public:
    /**
     * Takes a reference to a CountedGuard and maintains its internal counter based on the scope of this class.
     */
    class ScopedGuard {
      public:
        ScopedGuard() = delete;

        explicit ScopedGuard(CountedGuard& guard_counter) : guard_counter_(&guard_counter) {
            guard_counter_->counted_value_++;
        }

        ScopedGuard(const ScopedGuard& other) : guard_counter_(other.guard_counter_) {
            guard_counter_->counted_value_++;
        }

        ScopedGuard(ScopedGuard&& other) noexcept : guard_counter_(other.guard_counter_) {
            other.guard_counter_ = nullptr;
        }

        virtual ~ScopedGuard() {
            if (guard_counter_ && --guard_counter_->counted_value_ == 0) {
                if (guard_counter_->on_release) {
                    guard_counter_->on_release();
                }
            }
        }

        ScopedGuard& operator=(const ScopedGuard& other) {
            if (this == &other)
                return *this;

            guard_counter_ = other.guard_counter_;
            guard_counter_->counted_value_++;

            return *this;
        }

        ScopedGuard& operator=(ScopedGuard&& other) noexcept {
            guard_counter_->counted_value_--;
            guard_counter_ = other.guard_counter_;
            other.guard_counter_ = nullptr;

            return *this;
        }

      private:
        CountedGuard* guard_counter_ {nullptr};  // Wrapper to allow assignment of the actual reference.
    };

    /// Called then the guard counter drops to zero.
    std::function<void()> on_release;

    /**
     * Increases the counter by one, and returns a ScopedGuard which will decrease the counter by one upon destruction
     * and execute onRelease if the value dropped to 0. Make sure that CountedGuard outlives the returned ScopedGuard.
     */
    ScopedGuard get_guard() {
        return ScopedGuard(*this);
    }

    /**
     * @return Returns the current amount of guards active.
     */
    [[nodiscard]] int get_count() const {
        return counted_value_;
    }

  private:
    int counted_value_ = 0;
};

}  // namespace rdk
