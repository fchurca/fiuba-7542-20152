#ifndef _MODEL_ENTITY_FACTORY_H_
#define _MODEL_ENTITY_FACTORY_H_

#include <string>
#include <memory>

#include "board.h"
#include "entity.h"

class EntityFactory {
	public:
		const std::string name;
		const int size_x;
		const int size_y;
		const double speed;
		Board& board;

		EntityFactory(std::string name, int size_x, int size_y, double speed, Board& board);
		~EntityFactory();

		std::shared_ptr<Entity> createEntity(int x, int y);};

#endif // _MODEL_ENTITY_FACTORY_H_
