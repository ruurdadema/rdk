#pragma once

#include <cassert>
#include <functional>

namespace rdk
{

/**
 * A simple class which executes given function upon destruction.
 * Very suitable for subscriptions which must go out of scope when the owning class gets destructed.
 */
class Subscription
{
public:
    Subscription() = default;

    explicit Subscription (std::function<void()> onDestructionCallback) :
        mOnDestructionCallback (std::move (onDestructionCallback))
    {
    }

    Subscription (const Subscription& other) = delete;

    Subscription (Subscription&& other) noexcept { *this = std::move (other); }

    ~Subscription()
    {
        if (mOnDestructionCallback)
            mOnDestructionCallback();
    }

    explicit operator bool() const { return mOnDestructionCallback != nullptr; }

    Subscription& operator= (const Subscription& other) = delete;

    Subscription& operator= (Subscription&& other) noexcept
    {
        if (mOnDestructionCallback)
            mOnDestructionCallback();

        mOnDestructionCallback = std::move (other.mOnDestructionCallback);
        other.mOnDestructionCallback = nullptr;

        // Apparently a move doesn't reset the std::function
        assert (other.mOnDestructionCallback == nullptr);

        return *this;
    }

    Subscription& operator= (std::function<void()>&& onDestructionCallback) noexcept
    {
        if (mOnDestructionCallback)
            mOnDestructionCallback();

        mOnDestructionCallback = std::move (onDestructionCallback);

        return *this;
    }

    void reset()
    {
        if (mOnDestructionCallback)
        {
            mOnDestructionCallback();
            mOnDestructionCallback = nullptr;
        }
    }

    /**
     * Clears the destruction callback without invoking it.
     * Warning! This basically negates the point of this class, you should probably rarely use it, if ever.
     */
    void neutralize()
    {
        mOnDestructionCallback = nullptr;
    }

private:
    std::function<void()> mOnDestructionCallback;
};

} // namespace
