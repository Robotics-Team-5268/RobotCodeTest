#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>

/*
 * A condition_variable is a synchronization primitive that allows one or more
 * threads to be blocked until a shared state has been modified and the
 * condition_variable has been notified.
 *
 * Why are we using it?
 *      The blocking and notification pieces are helpful in this situation.
 * We want to execute periodically or temporarily suspend until awoken
 * again. This is exactly what a condition_variable is for.
 */
#include <condition_variable>
/*
 * A mutex is a synchronization primitive that allows programmers to protect
 * shared data from being simultaneously accessed from different threads. This
 * is known as mutal exclusion.
 *
 * Why are we using it?
 *      The condition_variable requires it because the condition_variable is
 * itself shared by multiple threads.
 */
#include <mutex>
/*
 * A thread allows programs to execute one or more functions concurrently.
 * The thread object may represent one of these concurrent states or be in
 * state not representative of any thread. This occurs when the thread
 * is default initialized, has been moved, or has terminated (e.g. - return).
 *
 * Why are we using it?
 *      The robot code runs in a loop that we only have limited control of
 * when an event occurs. That is fine during the match, but before the match
 * we need to check for autonomous inputs.
 */
#include <thread>

class SuspendableThread
{
public:
    /*
     * Defines the states the thread is allowed to be in.
     */
    enum State
    {
        Idle,           // Thread context exists but is inactive.
        Running,        // Thread context may execute concurrently.
        Terminated      // Thread context does not exist.
    };

    SuspendableThread();
    virtual ~SuspendableThread();

    std::uint32_t GetCount() const;

    /* Thread state mutators. */
    bool Suspend();
    bool Run();
    bool Terminate();

    template<
        typename Rep,
        typename Period = std::ratio<1>
    > void SetRunDelay(const std::chrono::duration<Rep, Period>& delay)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mRunDelay = std::chrono::duration_cast<std::chrono::milliseconds>(delay);
    }

    /*
     * Deleted functions. These prevent unintended construction and assignment.
     * The compiler can generate default operations but there can be side
     * effects (e.g. - copying of pointers instead of value pointed to) that
     * can lead to errors.
     */
    SuspendableThread(const SuspendableThread&) = delete;
    SuspendableThread(SuspendableThread&&) = delete;
    SuspendableThread& operator=(const SuspendableThread&) = delete;
    SuspendableThread& operator=(SuspendableThread&&) = delete;

protected:
    /*
     * This is a method that subclasss may override.
     */
    virtual void OnRun();

private:
    /*
     * The private members are those responsible for controlling the state
     * of the thread. To ensure a subclass cannot cause issues with the
     * basic functionality these members are hidden.
     */

    void DelayRun();
    void Loop();
    void SetState(State aNew);

    // Loop counter, just for debug.
    std::atomic<uint32_t> mCounter;
    // Synchronization primitves used to block the thread and protected
    // shared data like mRunDelay.
    std::condition_variable mCondVar;
    std::mutex mMutex;
    // Amount of time to wait between calls to OnRun().
    std::chrono::milliseconds mRunDelay;
    // Current state of this thread.
    std::atomic<State> mState;
    std::thread mThread;
};
