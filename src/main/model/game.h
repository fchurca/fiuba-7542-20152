#ifndef __GAME_H__
#define __GAME_H__

#include <map>
#include <string>
#include <mutex>

#include "abstract_client.h"
#include "../log/logger.h"
#include "../model/board.h"

#include "../gfx/game_timer.h"

class Game {
private:
	void init();
	std::shared_ptr<ABoard> board;
	std::map<std::string, std::shared_ptr<AClient>> clients;
	std::mutex clientsMutex;
	bool exit_p;
	bool restart_p;
	void clear();
public:
	GameTimer timer;
	Game();
	~Game();
	std::shared_ptr<ABoard> getBoard();
	bool setBoard(std::shared_ptr<ABoard> newBoard);
	std::shared_ptr<Player> getAvailablePlayer();
	bool addClient(std::shared_ptr<AClient> newClient);
	void start();
	void restart();
	bool willRestart();
	void exit();
	bool willExit();
};
#endif //__GAME_H__
