#include "abstract_client.h"
#include "player.h"

AClient::AClient(Game& owner, Player& player) :
	owner(owner),
	player(player)
{
}

