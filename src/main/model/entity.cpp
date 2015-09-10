#include "entity.h"
#include "board.h"
#include <iostream>

using namespace std;
//-----------------------------------------------------------------------------
Entity::Entity(std::string name, Board& board, double x, double y) :
	name(name),
	board(board) {
		this->x = x;
		this->y = y;
		speed = 1;
		adjustPosition();
		cerr << "Created Entity " << this
			<< " of kind " << name
			<< " owned by board " << &board
			<< " at " << x << "," << y << endl;
	}

void Entity::adjustPosition() {
	int topX = board.sizeX - 1;
	int topY = board.sizeY - 1;
	x = x < topX ? x : topX;
	x = x >= 0 ? x : 0;
	y = y < topY ? y : topY;
	y = y >= 0 ? y : 0;
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
