#ifndef _MODEL_ABSTRACTCLIENT_H_
#define _MODEL_ABSTRACTCLIENT_H_

#include "mixins.h"

class Game;
class Player;

class AClient : public DeletableMixin {
	public:
		virtual ~AClient();
		virtual void update() = 0;
		Game& owner;
		Player& player;
		AClient(Game& owner, Player& player);
		virtual void notifyDeath(int id);
};

#endif // _MODEL_ABSTRACTCLIENT_H_
