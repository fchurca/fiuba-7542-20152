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
		int sight_radius;
		bool solid;
		int capacity;
		ABoard& board;

		EntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual ~EntityFactory();

		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		virtual void populate();
};

class UnitFactory: public EntityFactory {
	public:
		UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
};

class WorkerFactory: public UnitFactory {
	public:
		WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
};
class KingFactory: public UnitFactory {
	public:
		KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
};

class StructureFactory: public EntityFactory {
	public:
		StructureFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
};

class ResourceFactory: public StructureFactory {
	public:
		ResourceFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		void populate();
};

class BuildingFactory: public StructureFactory {
	public:
		BuildingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		void populate();
};

class ProducerBuildingFactory: public StructureFactory {
	public:
		ProducerBuildingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		void populate();
};

#endif // _MODEL_ENTITY_FACTORY_H_
