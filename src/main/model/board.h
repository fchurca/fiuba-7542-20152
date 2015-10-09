#ifndef _MODEL_BOARD_H_
#define _MODEL_BOARD_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <memory>
//-----------------------------------------------------------------------------
#include "player.h"
#include "../log/logger.h"

class Entity;
class EntityFactory;
class ParserYAML;

class Board {

private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::map<std::string, std::shared_ptr<Player>> players;
	std::map<std::string, std::shared_ptr<EntityFactory>> entityFactories;
	Board();
	std::shared_ptr<Entity> protagonist;
	std::vector<std::shared_ptr<Entity>> terrain;

public:
	const int
		sizeX, sizeY;
	const long maxResources;
	size_t dt;
	Board(ParserYAML& parser);
	~Board();

	void setTerrain(std::string name, size_t x, size_t y);
	Entity & getTerrain(size_t x, size_t y);
	std::shared_ptr<Entity> findEntity(rectangle r);
	std::shared_ptr<Entity> createEntity(std::string name, std::string playerName, r2 position);
	std::shared_ptr<Player> createPlayer(std::string name);
	std::shared_ptr<EntityFactory> createEntityFactory(std::string name, r2 size, double speed, int sight_radius);
	void update();
	std::vector<std::shared_ptr<Entity>> getEntities();
	template<typename Pred>
		std::vector<std::shared_ptr<Entity>> selectEntities(Pred pred);
	Player& findPlayer(std::string name);
};

#include "board.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
