#include "TimeService.h"
#include "Log.h"

namespace asteroids
{
    TimeService::TimeService() :
        _elapsed(0.0f),
        _lastTime(0.0f)
    {}

    void TimeService::Reset()
    {
        Log::info("Resetting TimeService.");
        _elapsed = 0.0f;
        _lastReset = _lastTime = Now();
    }

    void TimeService::Update()
    {
        // Checks elapsed time since last frame. It is important to
        // work on double with current time to avoid losing accuracy
        // Then we can go back to float for elapsed time.
        double lCurrentTime = Now();
        _elapsed = (lCurrentTime - _lastTime);
        _lastTime = lCurrentTime;
    }

    double TimeService::Now()
    {
        // Retrieves current time with a monotonic clock to ensure
        // time goes forward and is not subject to system changes.
        timespec lTimeVal;
        clock_gettime(CLOCK_MONOTONIC, &lTimeVal);
        return lTimeVal.tv_sec + (lTimeVal.tv_nsec * 1.0e-9);
    }

    float TimeService::Elapsed()
    {
        return _elapsed;
    }

    float TimeService::TotalElapsed()
    {
        return Now() - _lastReset;
    }
}
