#ifndef _MODEL_PLAYER_H_
#define _MODEL_PLAYER_H_

#include <memory>
#include <string>
#include <vector>

class Board;
class Entity;

class Player {
	protected:
		std::string name;
	public:
		Player(Board& board, std::string name);
		Board& board;
		std::vector<std::shared_ptr<Entity>> entities();
		void update();
};

#endif
