#include <sstream>

#include "entity_factory.h"

EntityFactory::EntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board):
	name(name),
	size(size),
	speed(speed),
	board(board),
	sight_radius(sight_radius),
	solid(solid),
	capacity(capacity)
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

std::shared_ptr<Entity> EntityFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Entity>(name, board, player, position, size, speed, sight_radius, solid, capacity);
}

void EntityFactory::populate() {
}


UnitFactory::UnitFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board) :
	EntityFactory(name, size, speed, sight_radius, solid, capacity, board)
{}


StructureFactory::StructureFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board) :
	EntityFactory(name, size, speed, sight_radius, solid, capacity, board)
{}


ResourceFactory::ResourceFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, ABoard& board) :
	StructureFactory(name, size, speed, sight_radius, solid, capacity, board)
{}

std::shared_ptr<Entity> ResourceFactory::createEntity(Player& player, r2 position) {
	return std::make_shared<Resource>(name, board, player, position, size, speed, sight_radius, solid, capacity);
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

