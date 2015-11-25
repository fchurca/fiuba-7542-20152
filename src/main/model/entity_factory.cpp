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


UnitFactory::UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board),
	speed(speed),
	health(health)
{}

std::shared_ptr<Entity> UnitFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Unit>(name, board, player, position, size, speed, sight_radius, solid, health);
}


WorkerFactory::WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, std::map<std::string, std::map<std::string, unsigned int>> workerProducts , ABoard& board) :
	UnitFactory(name, size, speed, sight_radius, solid, health, board),
	products(workerProducts)
{}

std::shared_ptr<Entity> WorkerFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Worker>(name, board, player, position, size, speed, sight_radius, solid, health, products);
}


KingFactory::KingFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int health, ABoard& board) :
	UnitFactory(name, size, speed, sight_radius, solid, health, board)
{}

std::shared_ptr<Entity> KingFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<King>(name, board, player, position, size, speed, sight_radius, solid, health);
}


BuildingFactory::BuildingFactory(std::string name, r2 size, int sight_radius, bool solid, int health, ABoard& board) :
	EntityFactory(name, size, sight_radius, solid, board),
	health(health)
{}

std::shared_ptr<Entity> BuildingFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Building>(name, board, player, position, size, sight_radius, solid, health);
}


ProducerBuildingFactory::ProducerBuildingFactory(std::string name, r2 size, int sight_radius, bool solid, int health, std::map<std::string, std::map<std::string, unsigned int>> producerProducts, ABoard& board) :
	BuildingFactory(name, size, sight_radius, solid, health, board),
	products(producerProducts)
{}

std::shared_ptr<Entity> ProducerBuildingFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<ProducerBuilding>(name, board, player, position, size, sight_radius, solid, health, products);
}


FlagFactory::FlagFactory(std::string name, r2 size, int sight_radius, bool solid, int health, ABoard& board) :
	BuildingFactory(name, size, sight_radius, solid, health, board)
{}

std::shared_ptr<Entity> FlagFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Flag>(name, board, player, position, size, sight_radius, solid, health);
}


TownCenterFactory::TownCenterFactory(std::string name, r2 size, int sight_radius, bool solid, int health, std::map<std::string, std::map<std::string, unsigned int>> producerProducts, ABoard& board) :
	ProducerBuildingFactory(name, size, sight_radius, solid, health, producerProducts, board)
{}

std::shared_ptr<Entity> TownCenterFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<TownCenter>(name, board, player, position, size, sight_radius, solid, health, products);
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

