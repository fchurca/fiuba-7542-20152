#include "player.h"

#include "board.h"
#include "entity.h"

#include <algorithm>

using namespace std;

Player::Player(Board& board, std::string name, bool human) :
	board(board),
	name(name),
	human(human){
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
		e->mapVisible([this](r2 pos) {
					map_visibility[(int)pos.y* board.sizeX + (int)pos.x] = VISIBLE;
				});
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

std::map<std::string, long> Player::getResources() {
	return resources;
}

bool Player::canGrantResources(std::string resource, long r){
	auto& res = resources[resource];
	return (r <= 0 || -r < res) && r + res <= board.maxResources;
}

bool Player::grantResources(std::string resource, long r) {
	if (!canGrantResources(resource, r)) {
		return false;
	}
	resources[resource] += r;
	return true;
}

