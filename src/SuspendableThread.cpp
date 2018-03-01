#include "SuspendableThread.h"

using namespace std;

SuspendableThread::SuspendableThread()
    : mCounter{ 0u }
    , mCondVar{}
    , mMutex{}
    , mRunDelay{ 0u }
    , mState{ Idle }
    // This will start the thread. It is important that it be last in the
    // header file since the compiler will initialize variables in the
    // order they were declared.
    , mThread{ &SuspendableThread::Loop, this }
{
}

SuspendableThread::~SuspendableThread()
{
    Terminate();
}

std::uint32_t SuspendableThread::GetCount() const
{
    return mCounter;
}

bool SuspendableThread::Suspend()
{
    bool ret = false;
    switch (mState)
    {
    case Idle:
        // Already idle so just return success.
        ret = true;
        break;
    case Running:
        // Set to idle and return success.
        SetState(Idle);
        ret = true;
        break;
    default:
        // Thread is terminated or state is invalid
        // so indicate a failure.
        break;
    }
    return ret;
}

bool SuspendableThread::Run()
{
    bool ret = false;
    switch (mState)
    {
    case Idle:
        // Switch state to running.
        SetState(Running);
        ret = true;
        break;
    case Running:
        // Already running, nothing to do.
        ret = true;
        break;
    case Terminated:
        // Creating a thread can fail. When it does an exception is thrown.
        // This block allows us to catch the exception and react appropriately.
        try
        {
            mState = Running; // Why is this important?
            mThread = thread{ &SuspendableThread::Loop, this };
            ret = true;
        }
        // If the thread was not constructed then return state to
        // the proper value and indicate an error.
        catch (system_error)
        {
            mState = Terminated;
            ret = false;
        }
        break;
    default:
        // Invalid state.
        break;
    }
    return ret;
}

bool SuspendableThread::Terminate()
{
    bool ret = false;
    switch (mState)
    {
    case Idle:
        // Intentionally fall through, same action as Running.
    case Running:
        SetState(Terminated);
        if (mThread.joinable())
        {
            mThread.join();
        }
        ret = true;
        break;
    case Terminated:
        ret = true;
        break;
    default:
        break;
    }
    return ret;
}

void SuspendableThread::OnRun()
{
    // Nothing to do in the base class.
}

void SuspendableThread::DelayRun()
{
    // Aliases to reduce line length.
    using clock = chrono::steady_clock;
    using period = chrono::milliseconds;

    // Lock the mutex since the run delay is shared. In the event that
    // the run delay is not zero, it needs to be locked anyway.
    unique_lock<mutex> lock{ mMutex };
    if (0u != mRunDelay.count())
    {
        // Set the start time and amount of time to wait.
        clock::time_point start = clock::now();
        period remaining(mRunDelay);
 
        // The mutex owned by lock needs to be locked when calling this
        // function. The function will unlock the mutex when the thread
        // blocks. Whenever the thread wakes up (e.g. - to check the
        // predicate) the mutex will be locked.
        mCondVar.wait_for
        (
            lock,
            remaining,
            // This ia a lambda function that captures local variables by
            // reference. It may modify start and remaining because
            // condition_variable::wait_for() may wakeup early.
            [&]()
            {
                // Calculate the elapsed time.
                clock::time_point cur = clock::now();
                period elapsed = chrono::duration_cast<period>(cur - start);
                if (elapsed < remaining)
                {
                    // Not enough time has passed so update the starting
                    // time and subtract the elapsed time from the
                    // remaining time.
                    start = cur;
                    remaining -= elapsed;
                }
                else
                {
                    // Otherwise, at least mRunDelay has passed.
                    remaining = period(0);
                }
                // Stop waiting if the state has changed or enough time has
                // passed.
                return (Running != mState) || (0 == remaining.count());
            }
        );
    }
}

void SuspendableThread::Loop()
{
    // The thread will keep running until it is terminated.
    while(Terminated != mState)
    {
        if (Running == mState)
        {
            ++mCounter;
            OnRun();
            DelayRun();
        }
        else if (Idle == mState)
        {
            // Wait until the state is not Idle.
            unique_lock<mutex> lock(mMutex);
            mCondVar.wait(lock, [this]() { return Idle != mState; });
        }
    }
}

void SuspendableThread::SetState(State aNew)
{
    mState = aNew;
    // Wakes up the thread if is blocked on mCondVar. This wil allow
    // it to detect state changes while making Idle very efficient.
    mCondVar.notify_one();
}
