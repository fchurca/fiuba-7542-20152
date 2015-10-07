#include <iostream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"

using namespace std;

Entity::Entity(std::string name, Board& board, Player& owner, r2 position, r2 size, double speed, int sight_radius) :
	position(position),
	speed(speed),
	deletable(false),
	size(size),
	name(name),
	owner(owner),
	board(board),
	sight_radius(sight_radius)
{
	static size_t idCount = 0;
	id = idCount++;
	adjustPosition();
	stringstream message;
	message << "Created Entity " << this
		<< " with ID " << id
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
	position = {clip(position.x, 0, topX),clip(position.y, 0, topY)};
	return oldpos != position;
}

void Entity::addTarget(r2 newTarget) {
	targets.push_back(newTarget);
}

void Entity::unsetTarget() {
	targets.clear();
}

r2 Entity::target() {
	return targets.size() > 0?
		targets.front():
		r2(0, 0);
}

bool Entity::targeted() {
	return targets.size() > 0;
}

void Entity::update() {
	if (targeted()) {
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			auto newPos = position + r2::fromPolar(bearing(), dr);
			// TODO: colisionar
			auto colliders = board.selectEntities([this, newPos](shared_ptr<Entity> e) {
					return (*e != *this) &&
					(rectangle(newPos, size).intersects(rectangle(e->position, e->size)));
					});
			for(size_t i = 0; i < colliders.size();) {
				auto& c = *colliders[i];
				// TODO: pasar a e.collide()
				if(c.name == "carne") {
					cerr << "Un " << name << " de " << owner.name << " encontró carne!" << endl;
					colliders.erase(colliders.begin() + i);
					c.setDeletable();
					owner.grantResources(100);
					cerr << owner.name << " tiene " << owner.getResources() << " carne" << endl;
				} else {
					i++;
				}
			}
			if (colliders.size() > 0) {
				return;
			}
			position = newPos;
		} else {
			position = target() - size/2;
			targets.pop_front();
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

// TODO: Deprecar
double Entity::getX() {
	return position.x;
}

// TODO: Deprecar
double Entity::getY() {
	return position.y;
}

r2 Entity::trajectory() {
	return target() - center();
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
	return targeted()?
		static_cast<Directions>(
				(unsigned)floor(4*bearing()/M_PI+.5)%8):
		SOUTH_EAST;
}

void Entity::setDeletable() {
	deletable = true;
}

bool Entity::getDeletable() {
	return deletable;
}

size_t Entity::getId() {
	return id;
}

bool Entity::operator==(Entity& other) {
	return this == &other;
}

bool Entity::operator!=(Entity& other) {
	return this != &other;
}

