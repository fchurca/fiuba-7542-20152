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
	bool exit_p;
	bool restart_p;
	void clear();
public:
	Game();
	~Game();
	std::shared_ptr<Board> getBoard();
	bool setBoard(std::shared_ptr<Board> newBoard);
	std::shared_ptr<Player> getAvailablePlayer();
	bool addClient(std::shared_ptr<GameWindow> newClient);
	void start();
	void restart();
	bool willRestart();
	void exit();
	bool willExit();
};
#endif //__GAME_H__
