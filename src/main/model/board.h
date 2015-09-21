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
#include "../log/logger.h"

class Entity;
class EntityFactory;

class Board {

private:
	std::vector<std::shared_ptr<Entity>> entities;
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
	std::shared_ptr<Entity> createEntity(std::string name, double x, double y);
	std::shared_ptr<EntityFactory> createEntityFactory(std::string name, double size_x, double size_y, double speed);
	std::shared_ptr<Entity> createProtagonist(std::string name, double x, double y);
	void update();
	void buildBoard();
	std::vector<std::shared_ptr<Entity>> getEntities();
	Entity & getProtagonist();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
