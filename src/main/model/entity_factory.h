#ifndef _MODEL_ENTITY_FACTORY_H_
#define _MODEL_ENTITY_FACTORY_H_

#include <string>
#include <memory>

#include "board.h"
#include "entity.h"

class EntityFactory {
	public:
		const std::string name;
		const r2 size;
		const int sight_radius;
		const bool solid;
		ABoard& board;

		EntityFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board);
		virtual ~EntityFactory();

		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position) =0;
		virtual void populate();
};

class UnitFactory: public EntityFactory {
	public:
		const double speed;
		int health;
		int armour;
		int hitForce;
		int hitRadius;
		UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, int armour, int hit_force, int hit_radius, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class WorkerFactory: public UnitFactory {
	public:
		WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, int armour, int hit_force, int hit_radius, std::vector<Budget> workerProducts, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		std::vector<Budget> products;
};

class KingFactory: public UnitFactory {
	public:
		KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, int armour, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class ResourceFactory: public EntityFactory {
	public:
		const int capacity;
		std::string resource_name;
		ResourceFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, std::string resource_name, ABoard& board);
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
		int health;
		int armour;
		BuildingFactory(std::string name, r2 size, int sight_radius, bool solid, int health, int armour, std::vector<Budget> producerProducts, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		virtual std::shared_ptr<Entity> createUnfinished(Player& player, r2 position);
		std::vector<Budget> products;
};


class FlagFactory: public EntityFactory {
	public:
		int health;
		int armour;
		FlagFactory(std::string name, r2 size, int sight_radius, bool solid, int health, int armour, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class TownCenterFactory: public BuildingFactory {
	public:
		TownCenterFactory(std::string name, r2 size, int sight_radius, bool solid, int health, int armour, std::vector<Budget> producerProducts, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

#endif // _MODEL_ENTITY_FACTORY_H_
