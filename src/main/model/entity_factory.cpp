#include <sstream>

#include "entity_factory.h"

EntityFactory::EntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, Board& board):
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
	if(name == "carne" || name == "oro") {
		return std::make_shared<ResourceEntity>(name, board, player, position, size, speed, sight_radius, solid, capacity);
	} else {
		return std::make_shared<Entity>(name, board, player, position, size, speed, sight_radius, solid, capacity);
	}
}

void EntityFactory::populate() {
}

ResourceEntityFactory::ResourceEntityFactory(std::string name, r2 size, double speed, int sight_radius, bool solid, int capacity, Board& board) :
	EntityFactory(name, size, speed, sight_radius, solid, capacity, board)

{}

void ResourceEntityFactory::populate() {
	for(size_t n = board.sizeX*board.sizeY/10; n > 0; n--) {
		if ((rand()%2) > 0) {
			board.createEntity(name,
					DEFAULT_PLAYER_NAME,
					r2((double)(rand() % board.sizeX),
						(double)(rand() % board.sizeY)));
		}
	}
}

