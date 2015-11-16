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
		const double speed; // TODO: Belongs in UnitFactory
		int sight_radius;
		bool solid;
		int capacity;
		ABoard& board;

		EntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual ~EntityFactory();

		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position); // TODO: This should be =0
		virtual void populate();
};

class UnitFactory: public EntityFactory {
	public:
		UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class WorkerFactory: public UnitFactory {
	public:
		WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class KingFactory: public UnitFactory {
	public:
		KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class StructureFactory: public EntityFactory {
	public:
		StructureFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board);
};

class ResourceFactory: public StructureFactory {
	public:
		ResourceFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		void populate();
};

class BuildingFactory: public StructureFactory {
	public:
		BuildingFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class FlagFactory: public StructureFactory {
	public:
		FlagFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class ProducerBuildingFactory: public StructureFactory {
	public:
		ProducerBuildingFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

#endif // _MODEL_ENTITY_FACTORY_H_
