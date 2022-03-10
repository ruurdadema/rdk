//
// Created by Ruurd Adema on 29/11/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include "ICallbackProvider.h"

#include <functional>

namespace rdk
{

/**
 * Class which is able to execute work on a specific AsyncCallbackProvider.
 */
class AsyncExecutor : public ICallbackProvider::CallbackTarget
{
public:
    explicit AsyncExecutor (ICallbackProvider& asyncExecutor) : CallbackTarget (asyncExecutor) {}

    /**
     * Implemented to execute scheduled work.
     */
    void callbackProviderCallback() override
    {
        auto lock = getLockGuard();

        for (auto& work : mScheduledWork)
            if (work)
                work();

        mScheduledWork.clear();
    }

protected:
    /**
     * Schedules some work async using the set AsyncCallbackProvider.
     * @param work Work to execute.
     */
    void scheduleWork (std::function<void()> work)
    {
        auto lock = getLockGuard();

        mScheduledWork.push_back (std::move (work));
        scheduleCallback();
    }

private:
    std::vector<std::function<void()>> mScheduledWork;
};

} // namespace rdk
