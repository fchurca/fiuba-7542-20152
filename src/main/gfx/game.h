#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "../log/logger.h"
#include "../model/board.h"

class Game {
private:
	void init();
	Board* gameBoard;
public:
	Game(int sizeX, int sizeY, size_t dt);
	~Game();
	Board* getBoard();
	void update();
};
#endif //__GAME_H__