#include "entity.h"
#include <iostream>

using namespace std;
//-----------------------------------------------------------------------------
Entity::Entity(std::string name, Board& board, double x, double y) :
	name(name),
	board(board) {
		this->x = x;
		this->y = y;
		cerr << "Created Entity " << this
			<< " of kind " << name
			<< " owned by board " << &board
			<< " at " << x << "," << y << endl;
	}
//-----------------------------------------------------------------------------
Entity::~Entity() {
	cerr << "Killing Entity " << name << " " << this << endl;
}
//-----------------------------------------------------------------------------

void Entity::update() {
	cerr << "Entity " << this << " is alive at "
		<< x << "," << y << endl;
}
