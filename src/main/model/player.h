#ifndef _MODEL_PLAYER_H_
#define _MODEL_PLAYER_H_

#include <string>

class Player {
	protected:
		std::string name;
	public:
		Player(std::string name) : name(name) {};
};

#endif
