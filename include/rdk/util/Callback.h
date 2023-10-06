#pragma once

#include <functional>

namespace rdk
{

template <class ReturnType, class... ArgTypes> class Callback
{
};

template <class ReturnType, class... ArgTypes> class Callback<ReturnType (ArgTypes...)>
{
public:
    Callback() = default;

    ReturnType operator() (ArgTypes... args)
    {
        if (mCallback)
            return mCallback (args...);
        return {};
    }

    explicit operator bool() const { return mCallback; }

    Callback& operator= (const std::function<ReturnType (ArgTypes...)>& cb)
    {
        mCallback = cb;
        return *this;
    }

    void clear() { mCallback = nullptr; }

private:
    std::function<ReturnType (ArgTypes...)> mCallback {};
};

/**
 * Specialise for void return type
 * @tparam ArgTypes
 */
template <class... ArgTypes> class Callback<void (ArgTypes...)>
{
public:
    Callback() = default;

    void operator() (ArgTypes... args)
    {
        if (mCallback)
            mCallback (args...);
    }

    explicit operator bool() const { return mCallback; }

    Callback& operator= (const std::function<void (ArgTypes...)>& cb)
    {
        mCallback = cb;
        return *this;
    }

    Callback& operator= (std::function<void (ArgTypes...)>&& cb)
    {
        mCallback = std::move (cb);
        return *this;
    }

    void clear() { mCallback = nullptr; }

private:
    std::function<void (ArgTypes...)> mCallback {};
};

} // namespace rdk
