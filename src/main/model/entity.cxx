#include "board.h"

template<typename L>
void Entity::applyVisible(L fun) {
	auto c = center();
	c = r2(floor(c.x), floor(c.y));
	auto r = sight_radius + .5;
	for (int x = max(0, (int)floor(c.x - r - 1)); x <= min(board.sizeX - 1, (int)ceil(c.x + r + 1)); x++) {
		for (int y = max(0, (int)floor(c.y - r - 1)); y <= min(board.sizeY - 1, (int)ceil(c.y + r + 1)); y++) {
			r2 pos((double)x, (double)y);
			if ((pos - c).sqLength() < r*r) {
				fun(pos);
			}
		}
	}
}

