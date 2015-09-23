#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"

using namespace std;

Entity::Entity(std::string name, Board& board, r2 position, r2 size, double speed) :
	position(position),
	speed(speed),
	targeted(false),
	size(size),
	name(name),
	board(board)
{
	adjustPosition();
	stringstream message;
	message << "Created Entity " << this
		<< " of kind " << name
		<< " owned by board " << &board
		<< " at " << position.x << "," << position.y;
	Logger::getInstance()->writeInformation(message.str());
}

Entity::~Entity() {
	stringstream message;
	message << "Killing Entity " << this << " of kind " << name;
	Logger::getInstance()->writeInformation(message.str());
}

bool Entity::adjustPosition() {
	double topX = board.sizeX - size.x;
	double topY = board.sizeY - size.y;
	r2 oldpos = position;
	position.x = clip(position.x, 0, topX);
	position.y = clip(position.y, 0, topY);
	return oldpos != position;
}

void Entity::setTarget(r2 newTarget) {
	targeted = true;
	target = newTarget;
}

void Entity::unsetTarget() {
	targeted = false;
}

void Entity::update() {
	if (targeted) {
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			position += r2::fromPolar(bearing(), dr);
		} else {
			position = target - size/2;
			targeted = false;
		}
		if (adjustPosition()) {
			unsetTarget();
		}
	}
}

r2 Entity::center() {
	return position + (size/2);
}

r2 Entity::getPosition() {
	return position;
}

double Entity::getX() {
	return position.x;
}

double Entity::getY() {
	return position.y;
}

r2 Entity::trajectory() {
	return target - center();
}

double Entity::bearing() {
	auto traj = trajectory();
	return atan2(traj.y, traj.x);
}

double Entity::sqDistance() {
	auto b = trajectory();
	return pow(b.x, 2) + pow(b.y, 2);
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

