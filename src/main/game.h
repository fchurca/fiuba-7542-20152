#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "log/logger.h"
#include "model/board.h"

class Game {
private:
	void init();
	void update();
	Board* gameBoard;
public:
	Game(Board* board);
	~Game();
	Board* getBoard();
};
#endif //__GAME_H__
