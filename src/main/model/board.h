#ifndef _MODEL_BOARD_H_
#define _MODEL_BOARD_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <memory>
//-----------------------------------------------------------------------------
#include "entity.h"
#include "entity_factory.h"
#include "player.h"
#include "../log/logger.h"

class Entity;
class EntityFactory;

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
	size_t dt;
	Board(int sizeX, int sizeY, size_t dt);
	~Board();

	void setTerrain(std::string name, size_t x, size_t y);
	Entity & getTerrain(size_t x, size_t y);
	std::shared_ptr<Entity> findEntity(rectangle r);
	std::shared_ptr<Entity> createEntity(std::string name, std::string playerName, r2 position);
	std::shared_ptr<Player> createPlayer(std::string name);
	std::shared_ptr<EntityFactory> createEntityFactory(std::string name, r2 size, double speed);
	void init();
	void update();
	std::vector<std::shared_ptr<Entity>> getEntities();
	template<typename Pred>
		std::vector<std::shared_ptr<Entity>> selectEntities(Pred pred);
	Entity & getProtagonist();
};

#include "board.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
