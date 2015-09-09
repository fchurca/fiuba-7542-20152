#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "controllers/event_handler.h"
#include "gfx/game_window.h"
#include "log/logger.h"
#include "model/board.h"

class Game {
private:
	bool exitGame;
	bool endOfGame();	
	void init();
	void processInput();
	void update();
	void render();

	Board* gameBoard;
public:
	Game(Board* board);
	int start();
	~Game();
};
#endif //__GAME_H__
