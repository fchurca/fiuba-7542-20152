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

#include "../parser_yaml/parser_yaml.h"
//-----------------------------------------------------------------------------

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
	unsigned dt;
	Board(int sizeX, int sizeY);
	~Board();

	void setTerrain(std::string name, size_t x, size_t y);
	Entity & getTerrain(size_t x, size_t y);
	std::shared_ptr<Entity> createEntity(std::string name, double x, double y);
	std::shared_ptr<EntityFactory> createEntityFactory(std::string name, double size_x, double size_y, double speed);
	void createProtagonist(std::string name, double x, double y);
	void update();
	void buildBoard(ParserYAML* parser);
	std::vector<std::shared_ptr<Entity>> getEntities();
	Entity & getProtagonist();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
