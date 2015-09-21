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
	x(x), y(y),
	speed(speed),
	sizeX(sizeX), sizeY(sizeY)
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
	int oldX = x, oldY = y;
	int topX = board.sizeX;
	int topY = board.sizeY;
	if (x > topX - sizeX) {
		x = topX - sizeX;
		ret = true;
	} else if (x < 0) {
		x = 0;
		ret = true;
	}
	if (y > topY - sizeY) {
		y = topY - sizeY;
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
	if (targeted) {
		auto d = distance();
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			auto dx = cos(bearing())*dr;
			auto dy = sin(bearing())*dr;
			x += dx;
			y += dy;
		} else {
			x = targetX - sizeX/2;
			y = targetY - sizeY/2;
			targeted = false;
		}
		if (adjustPosition()) {
			unsetTarget();
		}
	}
}

double Entity::cX() {
	return x + sizeX/2;
}

double Entity::cY() {
	return y + sizeY/2;
}

double Entity::getX() {
	return this->x;
}

double Entity::getY() {
	return this->y;
}

double Entity::bearingX() {
	return targetX - cX();
}

double Entity::bearingY() {
	return targetY - cY();
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
