//
// Created by Ruurd Adema on 21/01/2022.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include "Subscription.h"
#include "rdk/detail/NonCopyable.h"
#include <functional>

namespace rdk
{

/**
 * Class which holds a counter and optionally a function which gets called when the counter drops to 0.
 */
class CountedGuard : NonCopyable
{
public:
    /**
     * Takes a reference to a CountedGuard and maintains its internal counter based on the scope of this class.
     */
    class ScopedGuard
    {
    public:
        ScopedGuard() = delete;

        explicit ScopedGuard (CountedGuard& guardCounter) : mGuardCounter (&guardCounter)
        {
            mGuardCounter->mCountedValue++;
        }

        ScopedGuard (const ScopedGuard& other) : mGuardCounter (other.mGuardCounter)
        {
            mGuardCounter->mCountedValue++;
        }

        ScopedGuard (ScopedGuard&& other) noexcept : mGuardCounter (other.mGuardCounter)
        {
            other.mGuardCounter = nullptr;
        }

        virtual ~ScopedGuard()
        {
            if (mGuardCounter && --mGuardCounter->mCountedValue == 0)
                if (mGuardCounter->onRelease)
                    mGuardCounter->onRelease();
        }

        ScopedGuard& operator= (const ScopedGuard& other)
        {
            if (this == &other)
                return *this;

            mGuardCounter = other.mGuardCounter;
            mGuardCounter->mCountedValue++;

            return *this;
        }

        ScopedGuard& operator= (ScopedGuard&& other) noexcept
        {
            mGuardCounter->mCountedValue--;
            mGuardCounter = other.mGuardCounter;
            other.mGuardCounter = nullptr;

            return *this;
        }

    private:
        CountedGuard* mGuardCounter { nullptr }; // Wrapper to allow assignment of the actual reference.
    };

    /// Called then the guard counter drops to zero.
    std::function<void()> onRelease;

    /**
     * Increases the counter by one, and returns a ScopedGuard which will decrease the counter by one upon destruction
     * and execute onRelease if the value dropped to 0. Make sure that CountedGuard outlives the returned ScopedGuard.
     */
    ScopedGuard getGuard()
    {
        return ScopedGuard (*this);
    }

    /**
     * @return Returns the current amount of guards active.
     */
    [[nodiscard]] int getCount() const
    {
        return mCountedValue;
    }

private:
    int mCountedValue = 0;
};

} // namespace rdk
