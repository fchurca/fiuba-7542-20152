#include <sstream>

#include "entity_factory.h"

EntityFactory::EntityFactory(std::string name, r2 size, double speed, int radius, Board& board):
	name(name),
	size(size),
	speed(speed),
	board(board),
	radius(radius)
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
	return std::make_shared<Entity>(name, board, player, position, size, speed, radius);
}

