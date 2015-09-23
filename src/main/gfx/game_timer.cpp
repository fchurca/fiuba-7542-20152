#include "game_timer.h"

int GameTimer::currentTime = 0;
int GameTimer::lastTime= 0;
int GameTimer::diffTime= 0;

int GameTimer::getCurrent(){
	return currentTime;
}

int GameTimer::getDiffTime(){
	return diffTime;
}

void GameTimer::update(){
	lastTime = currentTime;
	currentTime = SDL_GetTicks();	//Tiempo actual del juego en milisegundos;
	diffTime = currentTime - lastTime;	
}

bool GameTimer::wait(int target) {
	int dt = target - SDL_GetTicks();
	if(dt < 0) {
		return false;
	}
	SDL_Delay(dt);
	return true;
}

