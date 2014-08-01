#ifndef _PACKT_TIMESERVICE_HPP_
#define _PACKT_TIMESERVICE_HPP_

#include "Constants.h"

#include <time.h>

namespace asteroids
{
	// Probably one of the main thing in game: processes operations with time.
    class TimeService
    {
    public:
        TimeService();

        void Reset();
        void Update();

        double Now();
        float Elapsed();
        float TotalElapsed();

    private:
        float _elapsed;
        double _lastTime;
        double _lastReset;
    };
}
#endif
