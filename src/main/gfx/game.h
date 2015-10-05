#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include "../log/logger.h"
#include "../model/board.h"

class GameWindow;

class Game {
private:
	void init();
	std::shared_ptr<Board> board;
	std::shared_ptr<GameWindow> gameWindow;
public:
	Game();
	~Game();
	std::shared_ptr<Board> getBoard();
	void start();
	void update();
};
#endif //__GAME_H__
