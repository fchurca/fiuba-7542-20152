#ifndef _MODEL_ENTITY_FACTORY_H_
#define _MODEL_ENTITY_FACTORY_H_

#include <string>
#include <memory>

#include "board.h"
#include "entity.h"

class EntityFactory {
	public:
		const std::string name;
		const double size_x;
		const double size_y;
		const double speed;
		Board& board;

		EntityFactory(std::string name, double size_x, double size_y, double speed, Board& board);
		~EntityFactory();

		std::shared_ptr<Entity> createEntity(double x, double y);};

#endif // _MODEL_ENTITY_FACTORY_H_
