#ifndef _MODEL_ENTITY_FACTORY_H_
#define _MODEL_ENTITY_FACTORY_H_

#include <string>
#include <memory>

#include "board.h"
#include "entity.h"

class ResourceFactory;

class EntityFactory {
	public:
		const std::string name;
		r2 size;
		const double speed;
		int sight_radius;
		bool solid;
		int capacity;
		ABoard& board;

		EntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		~EntityFactory();

		std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		virtual void populate();
};

class ResourceFactory: public EntityFactory {
	public:
		ResourceFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		void populate();
};

#endif // _MODEL_ENTITY_FACTORY_H_
