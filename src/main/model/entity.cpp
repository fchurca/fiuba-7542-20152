#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"

using namespace std;

Entity::Entity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int capacity) :
	position(position),
	speed(speed),
	deletable(false),
	orientation(0),
	size(size),
	name(name),
	owner(owner),
	board(board),
	sight_radius(sight_radius),
	solid(solid),
	capacity(capacity)
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
	bool adjusted = oldpos != position;
	if (adjusted) {
		setFrame();
	}
	return adjusted;
}

void Entity::addTarget(r2 newTarget) {
	waypoints.push_back(newTarget);
}

void Entity::unsetTarget() {
	waypoints.clear();
}

r2 Entity::target() {
	return waypoints.size() > 0?
		waypoints.front():
		r2(0, 0);
}

bool Entity::targeted() {
	return waypoints.size() > 0;
}

void Entity::collide(Entity* other) {
	if(other) {
		if(!deletable &&
				!other->deletable) {
			other->collide(*this);
		}
	}
}

void Entity::collide(Entity& other) {}

void Entity::collide(ResourceEntity& other) {}

bool Entity::canEnter(r2 newPosition) {
	auto newCenter = newPosition + size / 2;
	if (newCenter.x < 0 ||
			newCenter.y < 0 ||
			newCenter.x >= board.sizeX ||
			newCenter.y >= board.sizeY) {
		return false;
	}
	if(board.getTerrain(floor(newCenter.x), floor(newCenter.y))->solid) {
		return false;
	}
	rectangle shapeCandidate(newPosition, size);
	auto colliders = board.selectEntities([this, shapeCandidate](shared_ptr<Entity> e) {
			return (*e != *this) &&
			e->solid &&
			!e->deletable &&
			(rectangle(e->position, e->size).intersects(shapeCandidate));
			});
	return colliders.size() == 0;
}

void Entity::update() {
	if (targeted()) {
		auto traj = trajectory();
		orientation = atan2(traj.y, traj.x);
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			auto newPos = position + r2::fromPolar(orientation, dr);
			// TODO: colisionar
			rectangle shapeCandidate(newPos, size);
			auto colliders = board.selectEntities([this, shapeCandidate](shared_ptr<Entity> e) {
					return (*e != *this) &&
					(rectangle(e->position, e->size).intersects(shapeCandidate));
					});
			for(auto c : colliders) {
				collide(c.get());
			}
			if (!canEnter(newPos)) {
				unsetTarget();
				return;
			}
			position = newPos;
		} else {
			position = target() - size/2;
			waypoints.pop_front();
		}
		if (adjustPosition()) {
			unsetTarget();
		}
		setFrame();
	}
}

r2 Entity::center() {
	return position + (size/2);
}

r2 Entity::getPosition() {
	return position;
}

r2 Entity::trajectory() {
	return target() - center();
}

double Entity::sqDistance() {
	return trajectory().sqLength();
}

double Entity::distance() {
	return trajectory().length();
}

Directions Entity::getDirection(){
	return static_cast<Directions>((unsigned)floor(4*orientation/M_PI+.5)%8);
}

void Entity::setDeletable() {
	deletable = true;
	setFrame();
}

bool Entity::getDeletable() {
	return deletable;
}

size_t Entity::getId() {
	return id;
}

void Entity::setFrame() {
	frame = board.getFrame();
}

size_t Entity::getFrame() {
	return frame;
}

bool Entity::operator==(Entity& other) {
	return this == &other;
}

bool Entity::operator!=(Entity& other) {
	return !operator==(other);
}

string Entity::serialize() {
	stringstream ret;
	ret << "E\t" << id << '\t' << name << '\t'
		<< frame << '\t'
		<< owner.getId() << '\t'
		<< position.x << '\t' << position.y << '\t'
		<< orientation << endl;
	return ret.str();
}

ResourceEntity::ResourceEntity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int capacity):Entity(name, board, owner, position, size, speed, sight_radius, solid, capacity)
{}

void ResourceEntity::collide(Entity& other) {
	if(!getDeletable() &&
			!other.getDeletable()) {
		stringstream message;
		message << "Un " << other.name << " de " << other.owner.name << " encontró" << name;
		if(other.owner.grantResources(name, capacity)) {
			setDeletable();
			message << "; ahora " << other.owner.name
				<< " tiene " << other.owner.getResources()[name]
				<< " " << name;
		} else {
			message << "; pero no puede tomarlos";
		}
		Logger::getInstance()->writeInformation(message.str());
	}
}

void ResourceEntity::collide(ResourceEntity& other) {}

