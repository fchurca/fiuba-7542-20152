#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <SDL2/SDL.h>

class EventHandler {
private:
	SDL_Event evento;
	static EventHandler * instance;
	EventHandler();
public:
	SDL_Event * getEvent();
	static EventHandler * getInstance();
	~EventHandler();
};
#endif // __EVENTHANDLER_H__