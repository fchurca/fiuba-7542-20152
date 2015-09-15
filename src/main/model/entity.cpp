#include "entity.h"
#include "board.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

using namespace std;

Entity::Entity(std::string name, Board& board, double x, double y) :
	name(name),
	board(board),
	targeted(false)
{
	this->x = x;
	this->y = y;
	this->speed = 1;
	adjustPosition();
	cerr << "Created Entity " << this
		<< " of kind " << name
		<< " owned by board " << &board
		<< " at " << x << "," << y << endl;
}

Entity::~Entity() {
	cerr << "Killing Entity " << this << " of kind " << name << endl;
}

bool Entity::adjustPosition() {
	bool ret = false;
	int oldX = x, oldY = y;
	int topX = board.sizeX - 1;
	int topY = board.sizeY - 1;
	if (x > topX) {
		x = topX;
		ret = true;
	} else if (x < 0) {
		x = 0;
		ret = true;
	}
	if (y > topY) {
		y = topY;
		ret = true;
	} else if (y < 0) {
		y = 0;
		ret = true;
	}
	return ret;
}

void Entity::setTarget(double x, double y) {
	targeted = true;
	targetX = x;
	targetY = y;
}

void Entity::unsetTarget() {
	targeted = false;
}

void Entity::update() {
	cerr << "Entity " << this << " of kind " << name
		<< " is alive at " << x << "," << y;
	if (targeted) {
		auto d = distance();
		cerr << " heading for " << targetX << "," << targetY
			<< " at " << speed << " tiles/s"
			<< " with " << distance() << " tiles to walk";
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			auto dx = cos(bearing())*dr;
			auto dy = sin(bearing())*dr;
			x += dx;
			y += dy;
		} else {
			cerr << ", reaching target";
			x = targetX;
			y = targetY;
			targeted = false;
		}
		if (adjustPosition()) {
			cerr << ", reaching the end of the map";
			unsetTarget();
		}
		cerr << ", stepping into " << x << "," << y;
	} else {
		cerr << ", standing still";
	}
	cerr << endl;
}

double Entity::getX() {
	return this->x;
}

double Entity::getY() {
	return this->y;
}

double Entity::bearingX() {
	return targetX - x;
}

double Entity::bearingY() {
	return targetY - y;
}

double Entity::bearing() {
	return atan2(bearingY(), bearingX());
}

double Entity::sqDistance() {
	return pow(bearingX(), 2) + pow(bearingY(), 2);
}

double Entity::distance() {
	return sqrt(sqDistance());
}

Directions Entity::getDirection(){
	return SOUTH_EAST; // TODO: Sacar cuando tengamos algún sprite que se mueva en todas direcciones
	return targeted?
		static_cast<Directions>(
				(unsigned)floor(16*bearing()/M_PI)%8):
		SOUTH_EAST;
}
