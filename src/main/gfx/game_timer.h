#ifndef __GAMETIMER_H__
#define __GAMETIMER_H__

#include <SDL2/SDL.h>

class GameTimer {
private:
	static int currentTime;
	static int lastTime;
	static int diffTime;
public:
	static int getDiffTime();
	static int getCurrent();
	static void update();
	static bool wait(int target);
};
#endif // __GAMETIMER_H__
