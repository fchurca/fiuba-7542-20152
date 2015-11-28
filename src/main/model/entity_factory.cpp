#include <sstream>

#include "entity_factory.h"

EntityFactory::EntityFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board):
	name(name),
	size(size),
	board(board),
	sight_radius(sight_radius),
	solid(solid)
{
	std::stringstream message;
	message << "Created EntityFactory " << this
		<< " of kind " << name
		<< " with size " << size.x << "x" << size.y
		<< " owned by board " << &board;
	Logger::getInstance()->writeInformation(message.str());
}

EntityFactory::~EntityFactory() {
	std::stringstream message;
	message << "Killing EntityFactory " << this
		<< " of kind " << name;
	Logger::getInstance()->writeInformation(message.str());
}

void EntityFactory::populate() {
}


UnitFactory::UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, unsigned int health, unsigned int armour, unsigned int hitforce, unsigned int hitradius, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board),
	speed(speed),
	armour(armour),
	health(health),
	hitForce(hitforce),
	hitRadius(hitradius)
{}

std::shared_ptr<Entity> UnitFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Unit>(name, board, player, position, size, speed, sight_radius, solid, health, armour, hitForce, hitRadius);
}


WorkerFactory::WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, unsigned int health, unsigned int armour, unsigned int hit_force, unsigned int hit_radius, std::vector<Budget> workerProducts , ABoard& board) :
	UnitFactory(name, size, speed, sight_radius, solid, health, armour, hit_force, hit_radius, board),
	products(workerProducts)
{}

std::shared_ptr<Entity> WorkerFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Worker>(name, board, player, position, size, speed, sight_radius, solid, health, armour, hitForce, hitRadius, products);
}


KingFactory::KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, unsigned int health, unsigned int armour, ABoard& board) :
	UnitFactory(name, size, speed, sight_radius, solid, health, armour, 0, 0, board)
{}

std::shared_ptr<Entity> KingFactory::createEntity(Player& player, r2 position) {
	player.playerKing =  std::make_shared<King>(name, board, player, position, size, speed, sight_radius, solid, health, armour, hitForce, hitRadius);
	return player.playerKing;
}


BuildingFactory::BuildingFactory(std::string name, r2 size, int sight_radius, bool solid, unsigned int health, unsigned int armour, std::vector<Budget> producerProducts, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board),
	health(health),
	armour(armour),
	products(producerProducts)
{}

std::shared_ptr<Entity> BuildingFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Building>(name, board, player, position, size, sight_radius, solid, health, armour, products);
}

std::shared_ptr<Entity> BuildingFactory::createUnfinished(Player& player, r2 position) {
	return std::make_shared<UnfinishedBuilding>(name, board, player, position, size, sight_radius, solid, health, 0);
}


FlagFactory::FlagFactory(std::string name, r2 size, int sight_radius, bool solid, unsigned int health, unsigned int armour, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board),
	health(health),
	armour(armour)
{}

std::shared_ptr<Entity> FlagFactory::createEntity(Player& player, r2 position) {
	player.playerFlag =  std::make_shared<Flag>(name, board, player, position, size, sight_radius, solid, health, armour);
	return player.playerFlag;
}


TownCenterFactory::TownCenterFactory(std::string name, r2 size, int sight_radius, bool solid, unsigned int health, unsigned int armour, std::vector<Budget> producerProducts, ABoard& board) :
	BuildingFactory(name, size, sight_radius, solid, health, armour, producerProducts, board)
{}

std::shared_ptr<Entity> TownCenterFactory::createEntity(Player& player, r2 position) {
	player.playerTownCenter =  std::make_shared<TownCenter>(name, board, player, position, size, sight_radius, solid, health, armour, products);
	return player.playerTownCenter;
}

TerrainFactory::TerrainFactory(std::string name, r2 size, int sight_radius, bool solid, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board)
{}

std::shared_ptr<Entity> TerrainFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Terrain>(name, board, player, position, size, sight_radius, solid);
}


ResourceFactory::ResourceFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, std::string resourceName, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board),
	capacity(capacity),
	resource_name(resourceName)
{}

std::shared_ptr<Entity> ResourceFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Resource>(name, board, player, position, size, sight_radius, solid, capacity, resource_name);
}

void ResourceFactory::populate() {
	for(size_t n = board.sizeX*board.sizeY/10; n > 0; n--) {
		if ((rand()%2) > 0) {
			board.createEntity(name,
					DEFAULT_PLAYER_NAME,
					r2((double)(rand() % board.sizeX),
						(double)(rand() % board.sizeY)));
		}
	}
}

