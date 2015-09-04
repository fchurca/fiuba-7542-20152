#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "controllers/event_handler.h"
#include "gfx/game_window.h"
#include "log/logger.h"

class Game {
private:
	bool exitGame;
	bool endOfGame();	
	void init();
	void processInput();
	void update();
	void render();
public:
	Game();
	int start();
	~Game();
};
#endif //__GAME_H__