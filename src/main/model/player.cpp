#include "player.h"

#include "board.h"
#include "entity.h"

using namespace std;

Player::Player(Board& board, std::string name) :
	board(board),
	name(name),
	resources(100)
{
	for (int i = 0; i < board.sizeX * board.sizeY; i++)
		map_visibility.push_back(INVISIBLE);
};

vector<shared_ptr<Entity>> Player::entities() {
	return board.selectEntities(
			[this](shared_ptr<Entity> e) { return (&(e->owner) == this);});
}

void Player::update() {
}

Visibility Player::getVisibility(int x, int y) {
	return map_visibility[y * board.sizeY + x];
}

long Player::getResources() {
	return resources;
}

bool Player::canGrantResources(long r) {
	return (r <= 0 || -r < resources) && r + resources <= board.maxResources;;
}

bool Player::grantResources(long r) {
	if (!canGrantResources(r)) {
		return false;
	}
	resources += r;
	return true;
}

