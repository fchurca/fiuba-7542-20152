#include "player.h"

#include "board.h"
#include "entity.h"

using namespace std;

Player::Player(Board& board, std::string name) :
	board(board),
	name(name),
	resources(0) // TODO: Resources iniciales
{
	for (int i = 0; i < board.sizeX * board.sizeY; i++)
		map_visibility.push_back(INVISIBLE);
};

vector<shared_ptr<Entity>> Player::entities() {
	return board.selectEntities(
			[this](shared_ptr<Entity> e) { return (&(e->owner) == this);});
}

void Player::update() {
	for (auto& v : map_visibility) {
		if (v == VISIBLE) {
			v = SEEN;
		}
	}
	vector<shared_ptr<Entity>> entitites = entities();
	for (auto& e : entitites) {
		auto c = e->center();
		auto r = e->sight_radius;
		for (int x = max(0, (int)floor(c.x - r)); x < min(board.sizeX, (int)ceil(c.x + r)); x++) {
			for (int y = max(0, (int)floor(c.y - r)); y < min(board.sizeY, (int)ceil(c.y + r)); y++) {
				map_visibility[y* board.sizeX + x] = VISIBLE;
			}
		}
	}
}

Visibility Player::getVisibility(Entity& e) {
	Visibility ret = INVISIBLE;
	for(int x = 0; x < e.size.x; x++) {
		for(int y = 0; y < e.size.y; y++) {
			ret = max(ret, getVisibility(e.getPosition() + r2(x, y)));
		}
	}
	return ret;
}

Visibility Player::getVisibility(r2 pos) {
	return map_visibility[(int)floor(pos.y) * board.sizeX + (int)floor(pos.x)];
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

