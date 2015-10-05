#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "../log/logger.h"
#include "../model/board.h"

class GameWindow;

class Game {
private:
	void init();
	std::shared_ptr<Board> gameBoard;
	GameWindow* gameWindow;
public:
	Game(GameWindow * gameWindow);
	~Game();
	std::shared_ptr<Board> getBoard();
	void update();
};
#endif //__GAME_H__
