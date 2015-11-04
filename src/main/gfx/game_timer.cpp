#include "game_timer.h"

#include <thread>

using namespace std;
using namespace std::chrono;

#include <iostream>

GameTimer::GameTimer() : currentTime(Clock::now()) {}

TimePoint GameTimer::getCurrent(){
	return currentTime;
}

bool GameTimer::elapse(long duration) {
	auto target = currentTime + Ms(duration);
	bool ret = target > Clock::now();
	if(ret) {
		this_thread::sleep_until(target);
	}
	currentTime = Clock::now();
	return ret;
}

