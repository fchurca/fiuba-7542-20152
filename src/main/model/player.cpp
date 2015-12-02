#include <sstream>
#include <algorithm>

#include "player.h"

#include "board.h"
#include "entity.h"

using namespace std;

Player::Player(ABoard& board, std::string name, bool human) :
	board(board),
	name(name),
	human(human),
	active(!human),
	alive(true)
{
	static size_t idCount = 1;
	id = idCount++;
	for (int i = 0; i < board.sizeX * board.sizeY; i++)
		map_visibility.push_back(INVISIBLE);
};

vector<shared_ptr<Entity>> Player::entities() {
	return board.selectEntities(
			[this](shared_ptr<Entity> e) { return (&(e->owner) == this);});
}

void Player::update() {
	if(alive) {
		visibilitMutex.lock();
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
		visibilitMutex.unlock();
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
	visibilitMutex.lock();
	if (pos.x < 0 || pos.x >= board.sizeX ||
		pos.y < 0 || pos.y >= board.sizeY) {
		return INVISIBLE;
	}
	auto ret = map_visibility[(int)floor(pos.y) * board.sizeX + (int)floor(pos.x)];
	visibilitMutex.unlock();
	return ret;
}

Visibility Player::getVisibility2(r2 pos) { //TODO: Nose xq tuve que duplicar este metodo, si uso getVisibility en los lugares donde uso ahora getVisibility2 habia error.
	//visibilitMutex.lock();
	if (pos.x < 0 || pos.x >= board.sizeX ||
		pos.y < 0 || pos.y >= board.sizeY) {
		return INVISIBLE;
	}
	auto ret = map_visibility[(int)floor(pos.y) * board.sizeX + (int)floor(pos.x)];
	//visibilitMutex.unlock();
	return ret;
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
	setFrame();
	return true;
}

bool Player::setResources(std::string resource, long r) {
	resources[resource] = r;
	setFrame();
	return true;
}

void Player::setFrame() {
	frame = board.getFrame();
}

void Player::setActive(bool newActive) {
	active = newActive;
	setFrame();
}

bool Player::getActive() {
	return active;
}

void Player::kill() {
	alive = false;
	setFrame();
	for (auto& entity : entities()) {
		if ((board.gameMode != DESTROY_FLAG) && dynamic_cast<Unit*>(entity.get())) {
			entity->setDeletable();
		}
	}
}

bool Player::getAlive() {
	return alive;
}

void Player::conquer(Player& p) {
	for (auto& entity : p.entities()) {
		entity->conquered(*this);
	}
}

