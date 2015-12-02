#include "abstract_client.h"
#include "board.h"
#include "player.h"

AClient::AClient(Game& owner, Player& player) :
	owner(owner),
	player(player)
{
}

AClient::~AClient() {
	if(player.board.getState() == ABoard::BoardState::running) {
		player.kill();
	}
}

void AClient::notifyDeath(int id) {
}

