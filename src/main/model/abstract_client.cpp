#include "abstract_client.h"
#include "player.h"

AClient::AClient(Game& owner, Player& player) :
	deletable(true),
	owner(owner),
	player(player)
{
}

void AClient::setDeletable() {
	deletable = true;
	player.setActive(false);
}

bool AClient::getDeletable() {
	return deletable;
}

