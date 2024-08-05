#pragma once

#include <cassert>
#include <functional>

namespace rdk {

/**
 * A simple class which executes given function upon destruction.
 * Very suitable for subscriptions which must go out of scope when the owning class gets destructed.
 */
class Subscription {
  public:
    Subscription() = default;

    explicit Subscription(std::function<void()> onDestructionCallback) :
        on_destruction_callback_(std::move(onDestructionCallback)) {}

    Subscription(const Subscription& other) = delete;

    Subscription(Subscription&& other) noexcept {
        *this = std::move(other);
    }

    ~Subscription() {
        if (on_destruction_callback_)
            on_destruction_callback_();
    }

    explicit operator bool() const {
        return on_destruction_callback_ != nullptr;
    }

    Subscription& operator=(const Subscription& other) = delete;

    Subscription& operator=(Subscription&& other) noexcept {
        if (on_destruction_callback_)
            on_destruction_callback_();

        on_destruction_callback_ = std::move(other.on_destruction_callback_);
        other.on_destruction_callback_ = nullptr;

        // Apparently a move doesn't reset the std::function
        assert(other.on_destruction_callback_ == nullptr);

        return *this;
    }

    Subscription& operator=(std::function<void()>&& onDestructionCallback) noexcept {
        if (on_destruction_callback_)
            on_destruction_callback_();

        on_destruction_callback_ = std::move(onDestructionCallback);

        return *this;
    }

    void reset() {
        if (on_destruction_callback_) {
            on_destruction_callback_();
            on_destruction_callback_ = nullptr;
        }
    }

    /**
     * Clears the destruction callback without invoking it.
     * Warning! This basically negates the point of this class, you should probably rarely use it, if ever.
     */
    void neutralize() {
        on_destruction_callback_ = nullptr;
    }

  private:
    std::function<void()> on_destruction_callback_;
};

using Defer = Subscription;

}  // namespace rdk
