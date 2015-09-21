#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"

using namespace std;

Entity::Entity(std::string name, Board& board, double x, double y, double sizeX, double sizeY, double speed) :
	targeted(false),
	name(name),
	board(board),
	position(x, y),
	speed(speed),
	size(sizeX, sizeY)
{
	adjustPosition();
	stringstream message;
	message << "Created Entity " << this
		<< " of kind " << name
		<< " owned by board " << &board
		<< " at " << x << "," << y;
	Logger::getInstance()->writeInformation(message.str());
}

Entity::~Entity() {
	stringstream message;
	message << "Killing Entity " << this << " of kind " << name;
	Logger::getInstance()->writeInformation(message.str());
}

bool Entity::adjustPosition() {
	bool ret = false;
	int topX = board.sizeX;
	int topY = board.sizeY;
	if (position.x > topX - size.x) {
		position.x = topX - size.x;
		ret = true;
	} else if (position.x < 0) {
		position.x = 0;
		ret = true;
	}
	if (position.y > topY - size.y) {
		position.y = topY - size.y;
		ret = true;
	} else if (position.y < 0) {
		position.y = 0;
		ret = true;
	}
	return ret;
}

void Entity::setTarget(double x, double y) {
	targeted = true;
	target.x = x;
	target.y = y;
}

void Entity::unsetTarget() {
	targeted = false;
}

void Entity::update() {
	if (targeted) {
		auto d = distance();
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			auto dx = cos(bearing())*dr;
			auto dy = sin(bearing())*dr;
			position.x += dx;
			position.y += dy;
		} else {
			position.x = target.x - size.x/2;
			position.y = target.y - size.y/2;
			targeted = false;
		}
		if (adjustPosition()) {
			unsetTarget();
		}
	}
}

double Entity::cX() {
	return position.x + size.x/2;
}

double Entity::cY() {
	return position.y + size.y/2;
}

double Entity::getX() {
	return position.x;
}

double Entity::getY() {
	return position.y;
}

double Entity::bearingX() {
	return target.x - cX();
}

double Entity::bearingY() {
	return target.y - cY();
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
	return targeted?
		static_cast<Directions>(
				(unsigned)floor(4*bearing()/M_PI+.5)%8):
		SOUTH_EAST;
}
