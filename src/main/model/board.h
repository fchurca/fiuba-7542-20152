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
	std::vector<Entity*> entities;
	std::map<std::string, std::shared_ptr<EntityFactory>> entityFactories;
	Board();

public:
	const int
		sizeX, sizeY;
	Board(int sizeX, int sizeY);
	~Board();

	void createEntity(std::string name, double x, double y);
	void createEntityFactory(std::string name, int size_x, int size_y, double speed);
	void update();
	void buildBoard(ParserYAML* parser);
	std::vector<Entity*> getEntities();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
