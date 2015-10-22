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

///////////////////////////////////////////////////////////////////////////////
// HIC SVNT DRACONES
///////////////////////////////////////////////////////////////////////////////

#include <queue>
#include <set>

struct ASNode {
	r2 position;
	double g, f;
	shared_ptr<ASNode> previous;
	ASNode(r2 position, double g, double f, shared_ptr<ASNode> previous) :
		position(position), g(g), f(f), previous(previous) {};
};

class compare {
	public:
		bool operator()(const shared_ptr<ASNode> a, const shared_ptr<ASNode> b) {
			return a->f > b->f;
		}
};

void Entity::addTarget(r2 newTarget) {
	auto round = [](r2 a) {return r2(floor(a.x)+.5, floor(a.y)+.5);};
	r2
		end = targeted() ? waypoints.back() : position,
		start = round(newTarget);

	priority_queue<ASNode, vector<shared_ptr<ASNode>>, compare> open;
	auto h = [&end](r2& p) {return (p - end).length();};
	auto f = [&h](ASNode n) {return h(n.position) + n.g;};
	bool closed[board.sizeX][board.sizeY];
	for(size_t i = 0; i < board.sizeX; i++) {
		for(size_t j = 0; j < board.sizeY; j++) {
			closed[i][j] = false;
		}
	}

	open.emplace(make_shared<ASNode>(start, 0, h(start), nullptr));
	while (open.size() > 0) {
		auto c = open.top();
		open.pop();
		auto cpos = c->position;
		if(closed[(int)floor(cpos.x)][(int)floor(cpos.y)]) {
			continue;
		}
		closed[(int)floor(cpos.x)][(int)floor(cpos.y)] = true;
		if ((int)cpos.x == (int)end.x && (int)cpos.y == (int)end.y) {
			for (auto p = c; p; p = p->previous) {
				auto pos = p->position;
				waypoints.push_back(p->position);
			}
			return;
		}
		for(auto y = cpos.y - 1; y <= cpos.y + 1; y++) {
			for(auto x = cpos.x - 1; x <= cpos.x + 1; x++) {
				auto p = round(r2(x, y));
				if ((p == cpos) ||
						!(canEnter(p) &&
							canEnter(rectangle::box(p - size/2, cpos - size/2, size)))) {
					continue;
				}
				auto n = make_shared<ASNode>(p, (cpos - p).length() + c->g, .0, c);
				n->f = f(*n);
				if (closed[(int)floor(p.x)][(int)floor(p.y)]) {
					continue;
				}
				while(n->previous?
						n->previous->previous?
						canEnter(rectangle::box(p, n->previous->previous->position, size))
						:false
						:false) {
					n->previous = n->previous->previous;
				}
				open.emplace(n);
			}
		}
	}
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

bool Entity::canEnter(rectangle r) {
	auto& p = r.position;
	auto& s = r.size;
	for (int dx = 0; dx < s.x + 1; dx++) {
		for (int dy = 0; dy < s.y + 1; dy++) {
			auto t = board.getTerrain(floor(p.x + dx), floor(p.y + dy));
			if (t) {
				if(t->solid) {
					return false;
				}
			}
		}
	}
	auto colliders = board.selectEntities([this, r](shared_ptr<Entity> e) {
			return (*e != *this) &&
			e->solid &&
			!e->deletable &&
			(rectangle(e->position, e->size).intersects(r));
			});
	return colliders.size() == 0;
}

bool Entity::canEnter(r2 newPosition) {
	auto newCenter = newPosition + size / 2;
	if (newCenter.x < 0 ||
			newCenter.y < 0 ||
			newCenter.x >= board.sizeX ||
			newCenter.y >= board.sizeY) {
		return false;
	}
	return canEnter(rectangle(newPosition, size));
}

void Entity::update() {
	if (targeted()) {
		auto traj = trajectory();
		orientation = atan2(traj.y, traj.x);
		auto dr = speed*board.dt/1000;
		if (pow(dr, 2) < sqDistance()) {
			auto newPos = position + r2::fromPolar(orientation, dr);
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

