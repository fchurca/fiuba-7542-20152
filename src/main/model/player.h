#ifndef _MODEL_PLAYER_H_
#define _MODEL_PLAYER_H_

#include <string>

class Board;

class Player {
	protected:
		std::string name;
		Board& board;
	public:
		Player(Board& board, std::string name) :
			board(board),
			name(name) {};
};

#endif
