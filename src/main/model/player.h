#ifndef _MODEL_PLAYER_H_
#define _MODEL_PLAYER_H_

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "geometry.h"

class Board;
class Entity;

typedef enum {
	INVISIBLE = 0,
	SEEN = 1,
	VISIBLE = 2,
} Visibility;

class Player {
	protected:
		std::vector<Visibility> map_visibility;
		std::map<std::string, long> resources;
	public:
		const std::string name;
		const bool human;
		Player(Board& board, std::string name, bool human);
		Board& board;
		std::vector<std::shared_ptr<Entity>> entities();
		void update();
		Visibility getVisibility(r2 pos);
		Visibility getVisibility(Entity& e);
		std::map<std::string, long> getResources();
		bool canGrantResources(std::string resource, long r);
		bool grantResources(std::string resource, long r);
};

#endif
