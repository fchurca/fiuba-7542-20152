#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "log/logger.h"
#include "model/board.h"

class Game {
private:
	void init();
	Board* gameBoard;
public:
	Game(ParserYAML* parser);
	~Game();
	Board* getBoard();
	void update();
};
#endif //__GAME_H__
