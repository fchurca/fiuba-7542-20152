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
		int sight_radius;
		bool solid;
		ABoard& board;

		EntityFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
		virtual ~EntityFactory();

		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position) =0;
		virtual void populate();
};

class UnitFactory: public EntityFactory {
	public:
		const double speed;
		UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class WorkerFactory: public UnitFactory {
	public:
		WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class KingFactory: public UnitFactory {
	public:
		KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class ResourceFactory: public EntityFactory {
	public:
		int capacity;
		ResourceFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		void populate();
};

class TerrainFactory : public EntityFactory {
public:
	TerrainFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
	virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class BuildingFactory: public EntityFactory {
	public:
		BuildingFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
};

class FlagFactory: public BuildingFactory {
	public:
		FlagFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class ProducerBuildingFactory: public BuildingFactory { // TODO: products
	public:
		ProducerBuildingFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class TownCenterFactory: public ProducerBuildingFactory { // TODO: products
	public:
		TownCenterFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

#endif // _MODEL_ENTITY_FACTORY_H_
