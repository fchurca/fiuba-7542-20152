#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"

using namespace std;

Entity::Entity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	position(position),
	orientation(0),
	size(size),
	name(name),
	owner(owner),
	board(board),
	sight_radius(sight_radius),
	solid(solid)
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
	position = { clip(position.x, 0, topX),clip(position.y, 0, topY) };
	bool adjusted = oldpos != position;
	if (adjusted) {
		setFrame();
	}
	return adjusted;
}

void Entity::visit(EntityVisitor& e) {
	e.visit(*this);
}

///////////////////////////////////////////////////////////////////////////////
// HIC SVNT DRACONES
///////////////////////////////////////////////////////////////////////////////

#include <queue>
#include <set>

struct TSNode {
	r2 position;
	double g, f;
	shared_ptr<TSNode> previous;
	TSNode(r2 position, double g, double f, shared_ptr<TSNode> previous) :
		position(position), g(g), f(f), previous(previous) {};
};

class compare {
public:
	bool operator()(const shared_ptr<TSNode> a, const shared_ptr<TSNode> b) {
		return a->f > b->f;
	}
};

void Entity::addTarget(r2 newTarget) {
	newTarget.x = clip(newTarget.x, 0, board.sizeX);
	newTarget.y = clip(newTarget.y, 0, board.sizeY);
	auto round = [](r2 a) {return r2(floor(a.x) + .5, floor(a.y) + .5); };
	r2
		end = round(targeted() ? waypoints.back() : position),
		start = round(newTarget);

	priority_queue<TSNode, vector<shared_ptr<TSNode>>, compare> open;
	auto h = [&end](r2& p) {return (p - end).length(); };
	auto f = [&h](TSNode n) {return h(n.position) + n.g; };
	auto straightenOnce = [this](shared_ptr<TSNode> n) {
		if (n->previous) {
			if (n->previous->previous) {
				if (canEnter(rectangle::box(n->position, n->previous->previous->position, this->size))) {
					n->previous = n->previous->previous;
					return true;
				}
			}
		}
		return false;
	};
	auto straighten = [straightenOnce](shared_ptr<TSNode> n) {
		while (straightenOnce(n));
	};
	bool closed[board.sizeX][board.sizeY];
	for (size_t i = 0; i < board.sizeX; i++) {
		for (size_t j = 0; j < board.sizeY; j++) {
			closed[i][j] = false;
		}
	}

	open.emplace(make_shared<TSNode>(start, 0, h(start), nullptr));
	// i is a runtime cap
	for (size_t i = 4096; (open.size() > 0) && (i > 0); i--) {
		auto c = open.top();
		open.pop();
		auto cpos = c->position;
		if (cpos.x < 0 || cpos.x >= board.sizeX ||
			cpos.y < 0 || cpos.y >= board.sizeY) {
			continue;
		}
		if (closed[(int)floor(cpos.x)][(int)floor(cpos.y)]) {
			continue;
		}
		closed[(int)floor(cpos.x)][(int)floor(cpos.y)] = true;
		if ((cpos - end).sqLength() <= 1) {
			for (auto p = c; p; p = p->previous) {
				waypoints.push_back(p->position);
			}
			return;
		}
		for (auto y = cpos.y - 1; y <= cpos.y + 1; y++) {
			for (auto x = cpos.x - 1; x <= cpos.x + 1; x++) {
				if (x < 0 || x >= board.sizeX ||
					y < 0 || y >= board.sizeY) {
					continue;
				}
				auto p = r2(x, y);
				if (p == cpos) {
					continue;
				}
				if ((owner.getVisibility(p) != INVISIBLE)) {
					if (!(canEnter(rectangle::box(p - size / 2, cpos - size / 2, size)))) {
						continue;
					}
				}
				auto n = make_shared<TSNode>(p, (cpos - p).length() + c->g, .0, c);
				n->f = f(*n);
				if (closed[(int)floor(p.x)][(int)floor(p.y)]) {
					continue;
				}
				straighten(n);
				open.emplace(n);
			}
		}
	}
}

void Entity::unsetTarget() {
	waypoints.clear();
}

r2 Entity::target() {
	return waypoints.size() > 0 ?
		waypoints.front() :
		r2(0, 0);
}

bool Entity::targeted() {
	return waypoints.size() > 0;
}

void Entity::collide(Entity* other) {
	if (other) {
		if (!deletable &&
			!other->deletable) {
			other->collide(*this);
		}
	}
}

void Entity::collide(Entity& other) {}

void Entity::collide(Resource& other) {}

bool Entity::canEnter(rectangle r) {
	auto& p = r.position;
	auto& s = r.size;
	for (int dx = 0; dx < s.x + 1; dx++) {
		for (int dy = 0; dy < s.y + 1; dy++) {
			auto t = board.getTerrain(floor(p.x + dx), floor(p.y + dy));
			if (t) {
				if (t->solid) {
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
}

r2 Entity::center() {
	return position + (size / 2);
}

r2 Entity::getPosition() {
	return position;
}

void Entity::setPosition(r2 newPos) {
	position = newPos;
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

double Entity::getOrientation() {
	return orientation;
}

void Entity::setOrientation(double newOrientation) {
	orientation = newOrientation;
}

Directions Entity::getDirection() {
	return static_cast<Directions>((unsigned)floor(4 * orientation / M_PI + .5) % 8);
}

void Entity::setDeletable() {
	DeletableMixin::setDeletable();
	setFrame();
}

void Entity::setFrame() {
	setFrame(board.getFrame());
}

void Entity::setFrame(size_t newFrame) {
	frame = newFrame;
}

bool Entity::operator==(Entity& other) {
	return this == &other;
}

bool Entity::operator!=(Entity& other) {
	return !operator==(other);
}


Unit::~Unit() {}

Unit::Unit(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	speed(speed)
{}

bool Unit::getIsInAction() {
	return isInAction;
}

void Unit::update() {
	Entity::update();
	if (targeted()) {
		auto traj = trajectory();
		orientation = atan2(traj.y, traj.x);
		auto dr = speed*board.dt / 1000;
		if (pow(dr, 2) < sqDistance()) {
			auto newPos = position + r2::fromPolar(orientation, dr);
			rectangle shapeCandidate(newPos, size);
			auto colliders = board.selectEntities([this, shapeCandidate](shared_ptr<Entity> e) {
				return (*e != *this) &&
					(rectangle(e->getPosition(), e->size).intersects(shapeCandidate));
			});
			for (auto c : colliders) {
				collide(c.get());
			}
			if (!canEnter(newPos)) {
				auto destiny = waypoints.back();
				unsetTarget();
				addTarget(destiny);
				return;
			}
			position = newPos;
		}
		else {
			position = target() - size / 2;
			waypoints.pop_front();
		}
		if (adjustPosition()) {
			unsetTarget();
		}
		setFrame();
	}
}

void Unit::visit(EntityVisitor& e) {
	e.visit(*this);
}


Worker::Worker(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid) :
	Unit(name, board, owner, position, size, speed, sight_radius, solid)
{}

void Worker::update() {
	Unit::update();
}

void Worker::visit(EntityVisitor& e) {
	e.visit(*this);
}


King::King(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid) :
	Unit(name, board, owner, position, size, speed, sight_radius, solid)
{}

void King::update() {
	Unit::update();
}

void King::visit(EntityVisitor& e) {
	e.visit(*this);
}


Building::Building(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	Entity(name, board, owner, position, size, sight_radius, solid)
{}

void Building::update() {
	Entity::update();
}

Building::~Building() {
}

void Building::visit(EntityVisitor& e) {
	e.visit(*this);
}


UnfinishedBuilding::UnfinishedBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	Building(name, board, owner, position, size, sight_radius, solid)
{}

void UnfinishedBuilding::update() {
	Building::update();
}

void UnfinishedBuilding::visit(EntityVisitor& e) {
	e.visit(*this);
}


ProducerBuilding::ProducerBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	Building(name, board, owner, position, size, sight_radius, solid)
{}

void ProducerBuilding::update() {
	Building::update();
}

ProducerBuilding::~ProducerBuilding() {
}

void ProducerBuilding::visit(EntityVisitor& e) {
	e.visit(*this);
}


Flag::Flag(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	Building(name, board, owner, position, size, sight_radius, solid)
{}

void Flag::update() {
	Building::update();
}

void Flag::visit(EntityVisitor& e) {
	e.visit(*this);
}


TownCenter::TownCenter(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	ProducerBuilding(name, board, owner, position, size, sight_radius, solid)
{}

void TownCenter::update() {
	Building::update();
}

void TownCenter::visit(EntityVisitor& e) {
	e.visit(*this);
}

Terrain::Terrain(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	Entity(name, board, owner, position, size, sight_radius, solid)
{}

void Terrain::update() {
	Entity::update();
}

void Terrain::visit(EntityVisitor& e) {
	e.visit(*this);
}


Resource::Resource(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int capacity) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	capacity(capacity)
{}

void Resource::update() {
	Entity::update();
}

void Resource::collide(Entity& other) {
	if (!getDeletable() &&
		!other.getDeletable()) {
		stringstream message;
		message << "Un " << other.name << " de " << other.owner.name << " encontró" << name;
		if (other.owner.grantResources(name, capacity)) {
			setDeletable();
			message << "; ahora " << other.owner.name
				<< " tiene " << other.owner.getResources()[name]
				<< " " << name;
		}
		else {
			message << "; pero no puede tomarlos";
		}
		Logger::getInstance()->writeInformation(message.str());
	}
}

void Resource::collide(Resource& other) {}

void Resource::visit(EntityVisitor& e) {
	e.visit(*this);
}


void EntityVisitor::visit(Unit& u) {
	visit((Entity&)u);
}

void EntityVisitor::visit(Worker& w) {
	visit((Unit&)w);
}

void EntityVisitor::visit(King& k) {
	visit((Unit&)k);
}

void EntityVisitor::visit(Building& b) {
	visit((Entity&)b);
}

void EntityVisitor::visit(UnfinishedBuilding& u) {
	visit((Building&)u);
}

void EntityVisitor::visit(ProducerBuilding& p) {
	visit((Building&)p);
}

void EntityVisitor::visit(TownCenter& t) {
	visit((ProducerBuilding&)t);
}

void EntityVisitor::visit(Flag& f) {
	visit((Building&)f);
}

void EntityVisitor::visit(Resource& r) {
	visit((Entity&)r);
}

void EntityVisitor::visit(Terrain& r) {
	visit((Entity&)r);
}