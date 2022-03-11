//
// Created by Ruurd Adema on 11/03/2022.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <mutex>
#include <vector>

namespace rdk
{

/**
 * Class which can execute tasks on executor of type Executor.
 *
 * The Executor type must provide an interface like this:
 *
 *  class Executor
 *  {
 *  public:
 *      virtual void handleAsyncUpdate() = 0;
 *      void triggerAsyncUpdate();
 *  }
 *
 * @tparam Executor The type of executor to inherit from.
 */
template <class Executor>
class TaskScheduler : public Executor
{
public:
    /**
     * Constructor which can be given arguments which get passed to the Executor superclass.
     * @tparam Args Argument pack.
     * @param args Arguments which get passed.
     */
    template <class... Args>
    explicit TaskScheduler (Args&&... args) : Executor (std::forward<Args> (args)...)
    {
    }

    /**
     * Should be called when the Executor wants to handle work from whatever thread it wants.
     */
    void handleAsyncUpdate() override
    {
        std::lock_guard lg (mMutex);

        for (auto& work : mWork)
            if (work)
                work();

        mWork.clear();
    }

    /**
     * Schedules given work, and triggers an async update.
     * It is thread safe to schedule work.
     * @param work The work to schedule.
     */
    void scheduleWork (std::function<void()>&& work)
    {
        std::lock_guard lg (mMutex);
        mWork.push_back (std::move (work));
        Executor::triggerAsyncUpdate();
    }

private:
    std::recursive_mutex mMutex;
    std::vector<std::function<void()>> mWork;
};

} // namespace rdk
