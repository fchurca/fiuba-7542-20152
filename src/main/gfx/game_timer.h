#ifndef __GAMETIMER_H__
#define __GAMETIMER_H__

class GameTimer {
private:
	static long currentTime;
public:
	static long getCurrent();
	static void update();
	static bool waitUntil(long target);
	static bool elapse(long duration);
};

#endif // __GAMETIMER_H__
