#ifndef _MODEL_ENTITY_FACTORY_H_
#define _MODEL_ENTITY_FACTORY_H_

#include <string>
#include <memory>

#include "board.h"
#include "entity.h"

class EntityFactory {
	public:
		const std::string name;
		r2 size;
		const double speed;
		Board& board;

		EntityFactory(std::string name, r2 size, double speed, Board& board);
		~EntityFactory();

		std::shared_ptr<Entity> createEntity(r2 position);
};

#endif // _MODEL_ENTITY_FACTORY_H_
