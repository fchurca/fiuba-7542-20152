#ifndef _MODEL_ABSTRACTCLIENT_H_
#define _MODEL_ABSTRACTCLIENT_H_

class Game;
class Player;

class AClient{
	public:
		virtual void update() = 0;
		Game& owner;
		Player& player;
		AClient(Game& owner, Player& player);
};

#endif // _MODEL_ABSTRACTCLIENT_H_
