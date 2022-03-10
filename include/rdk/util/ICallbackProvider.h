//
// Created by Ruurd Adema on 29/11/2021.
// Copyright (c) Owllab. All rights reserved.
//

#pragma once

#include <mutex>
#include <vector>

namespace rdk
{

/**
 * This class provides an interface for classes which can callback from a certain thread.
 * Subclasses should implement signalDispatchThread which should trigger a call to dispatchCallbacks from the
 * corresponding thread/event-loop/executor.
 */
class ICallbackProvider
{
public:
    /**
     * Subscriber which is able to subscribe with ICallbackProvider and to be called by by it.
     */
    class CallbackTarget
    {
    public:
        explicit CallbackTarget (ICallbackProvider& callbackProvider) : mAsyncExecutor (callbackProvider) {}
        virtual ~CallbackTarget() { mAsyncExecutor.cancelCallbacksFor (this); }

        /**
         * Schedules an async callback with AsyncCallbackProvider.
         */
        void scheduleCallback() { mAsyncExecutor.scheduleAsyncCallback (this); }

        /**
         * Called when the AsyncCallbackProvider called back.
         */
        virtual void callbackProviderCallback() = 0;

    protected:
        std::lock_guard<std::recursive_mutex> getLockGuard() { return mAsyncExecutor.getLockGuard(); }

    private:
        ICallbackProvider& mAsyncExecutor;
    };

protected:
    /**
     * Should be called by the subclass from the correct thread/event-loop/executor.
     */
    void dispatchCallbacks()
    {
        std::lock_guard lock (mMutex);

        for (auto& callback : mScheduledCallbacks)
            if (callback)
                callback->callbackProviderCallback();

        mScheduledCallbacks.clear();
    }

    /**
     * Should be implemented by a subclass to let it know when to callback.
     */
    virtual void signalDispatchThread() = 0;

private:
    std::recursive_mutex mMutex;
    std::vector<CallbackTarget*> mScheduledCallbacks;

    /**
     * Schedules an async callback, if a callback for this subscriber is not already scheduled.
     * @param subscriberToCallback Subscriber to callback.
     */
    void scheduleAsyncCallback (CallbackTarget* subscriberToCallback)
    {
        std::lock_guard lock (mMutex);

        bool schedule = true;
        for (auto& s : mScheduledCallbacks)
            if (s == subscriberToCallback)
                schedule = false; // Already scheduled;

        if (schedule)
            mScheduledCallbacks.push_back (subscriberToCallback);

        signalDispatchThread();
    }

    /**
     * Cancels all callbacks which have been scheduled for subscriber.
     * @param subscriberToCancel Subscriber to cancel all scheduled callbacks for.
     */
    void cancelCallbacksFor (CallbackTarget* subscriberToCancel)
    {
        std::lock_guard lock (mMutex);

        mScheduledCallbacks.erase (
            std::remove_if (
                mScheduledCallbacks.begin(),
                mScheduledCallbacks.end(),
                [subscriberToCancel] (CallbackTarget* s) {
                    return s == subscriberToCancel;
                }),
            mScheduledCallbacks.end());
    }

    std::lock_guard<std::recursive_mutex> getLockGuard() { return std::lock_guard<std::recursive_mutex> { mMutex }; }
};

} // namespace rdk
