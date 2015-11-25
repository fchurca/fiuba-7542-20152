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
		const int health;
		unsigned int hit_force;
		unsigned int hit_radius;
		UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class WorkerFactory: public UnitFactory {
	public:
		WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius, std::map<std::string, std::map<std::string, unsigned int>> workerProducts, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		std::map<std::string, std::map < std::string, unsigned int >> products;
};

class KingFactory: public UnitFactory {
	public:
		KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, ABoard& board);
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
		const int health;
		BuildingFactory(std::string name, r2 size, int sight_radius, bool solid, int health, std::map<std::string, std::map < std::string, unsigned int >> producerProducts, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
		virtual std::shared_ptr<Entity> createUnfinished(Player& player, r2 position);
		std::map<std::string, std::map < std::string, unsigned int >> products;
};


class FlagFactory: public EntityFactory {
	public:
		const int health;
		FlagFactory(std::string name, r2 size, int sight_radius, bool solid, int health, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

class TownCenterFactory: public BuildingFactory {
	public:
		TownCenterFactory(std::string name, r2 size, int sight_radius, bool solid, int health, std::map<std::string, std::map < std::string, unsigned int >> producerProducts, ABoard& board);
		virtual std::shared_ptr<Entity> createEntity(Player& player, r2 position);
};

#endif // _MODEL_ENTITY_FACTORY_H_
