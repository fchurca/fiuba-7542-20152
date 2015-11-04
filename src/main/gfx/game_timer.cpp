#include "game_timer.h"

#include <SDL2/SDL.h>

using namespace std;

long GameTimer::currentTime = 0;

long GameTimer::getCurrent(){
	return currentTime;
}

void GameTimer::update(){
	currentTime = SDL_GetTicks();	//Tiempo actual del juego en milisegundos;
}

bool GameTimer::waitUntil(long target) {
	long dt = target - SDL_GetTicks();
	bool ret = (dt > 0);
	if (ret) {
		SDL_Delay(dt);
	}
	return true;
}

bool GameTimer::elapse(long duration) {
	return waitUntil(currentTime + duration);
}

