#include <iostream>

#include "entity_factory.h"

EntityFactory::EntityFactory(std::string name, int size_x, int size_y, double speed, Board& board):
	name(name),
	size_x(size_x), size_y(size_y),
	speed(speed),
	board(board)
{
	std::cerr << "Created EntityFactory " << this
		<< " of kind " << name
		<< " with size " << size_x << "x" << size_y
		<< " and speed " << speed << " tiles/s"
		<< " owned by board " << &board << std::endl;
}

EntityFactory::~EntityFactory() {
	std::cerr << "Killing EntityFactory " << this << std::endl;
}

Entity* EntityFactory::createEntity(int x, int y) {
	return new Entity(name, board, x, y);
}

