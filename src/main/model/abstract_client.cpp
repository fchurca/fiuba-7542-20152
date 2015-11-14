#include "abstract_client.h"
#include "player.h"

AClient::AClient(Game& owner, Player& player) :
	owner(owner),
	player(player)
{
}

AClient::~AClient() {}

void AClient::notifyDeath(int id) {
}

