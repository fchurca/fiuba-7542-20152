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
	for (int i = 0; i < board.sizeX * board.sizeY; i++) {
		if (map_visibility[i] == VISIBLE)
			map_visibility[i] = SEEN;
	}
	vector<shared_ptr<Entity>> entitites = entities();
	for (auto& e : entitites) {
		int x = e->center().x;
		int y = e->center().y;
		int j = y * board.sizeX + x;
		for (int rx = -e->sight_radius; rx <= e->sight_radius; rx++) {
			for (int ry = -e->sight_radius; ry <= e->sight_radius; ry++) {
				if ((y + ry) < board.sizeY && (x + rx) < board.sizeX && (y + ry) >= 0 && (x + rx) >= 0) {
					map_visibility[(y + ry) * board.sizeX + (x + rx)] = VISIBLE;
					if (name == "Franceses")
						Logger::getInstance()->writeError("Visible" + std::to_string((y + ry) * board.sizeX + (x + rx)));
				}
			}
		}
		if (name == "Franceses")
			Logger::getInstance()->writeError("-----------------------------------");
	}
}

Visibility Player::getVisibility(int x, int y) {
	return map_visibility[y * board.sizeX + x];
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

