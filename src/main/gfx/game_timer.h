#ifndef __GAMETIMER_H__
#define __GAMETIMER_H__

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Ms = std::chrono::milliseconds;

class GameTimer {
private:
	static TimePoint currentTime;
public:
	static TimePoint getCurrent();
	static bool elapse(long duration);
};

#endif // __GAMETIMER_H__
