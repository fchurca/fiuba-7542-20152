#include <sstream>

#include "entity_factory.h"

EntityFactory::EntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board):
	name(name),
	size(size),
	speed(speed), // TODO: Only in UnitFactory
	board(board),
	sight_radius(sight_radius),
	solid(solid),
	capacity(capacity) // TODO: Only in ResourceFactory
{
	std::stringstream message;
	message << "Created EntityFactory " << this
		<< " of kind " << name
		<< " with size " << size.x << "x" << size.y
		<< " and speed " << speed << " tiles/s"
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


UnitFactory::UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, ABoard& board) :
	EntityFactory(name, size, speed, sight_radius, solid, 0/*TODO: remove*/, board)
{}

std::shared_ptr<Entity> UnitFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Unit>(name, board, player, position, size, speed, sight_radius, solid);
}


WorkerFactory::WorkerFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board) :
	UnitFactory(name, size, speed, sight_radius, solid, board)
{}

std::shared_ptr<Entity> WorkerFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Worker>(name, board, player, position, size, speed, sight_radius, solid);
}


StructureFactory::StructureFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board) :
	EntityFactory(name, size, 0 /*TODO: remove*/, sight_radius, solid, capacity, board)
{}


ResourceFactory::ResourceFactory(std::string name, r2 size, int sight_radius, bool solid, int capacity, ABoard& board) :
	StructureFactory(name, size, sight_radius, solid, capacity, board)
{}

std::shared_ptr<Entity> ResourceFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Resource>(name, board, player, position, size, sight_radius, solid, capacity);
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

