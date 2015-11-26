#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"

using namespace std;


CargoMixin::CargoMixin(int max) :
	cargo(max)
{}

CargoMixin::CargoMixin(int max, int value) :
	cargo(max, value)
{}

CargoMixin::CargoMixin(int min, int max, int value) :
	cargo(min, max, value)
{}


HealthMixin::HealthMixin(int max) :
	health(max)
{}

HealthMixin::HealthMixin(int max, int value) :
	health(max, value)
{}

HealthMixin::HealthMixin(int min, int max, int value) :
	health(min, max, value)
{}


Entity::Entity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid) :
	position(position),
	orientation(0),
	command(nullptr),
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

void Entity::clearCommand() {
	setCommand(nullptr);
}

void Entity::setCommand(shared_ptr<Command> newCommand) {
	command = newCommand;
}

void Entity::update() {
	if (command) {
		cerr << this << " has a command: " << command << endl;
		command->execute(*this);
	}
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

void Entity::collide(Entity* other) {
	if (other) {
		if (!deletable &&
			!other->getDeletable()) {
			other->collide(*this);
		}
	}
}

double Entity::getOrientation() {
	return orientation;
}

void Entity::setOrientation(double newOrientation) {
	orientation = newOrientation;
}

// TODO: Rest of commands
#include <iostream>
void Entity::execute(MoveCommand& c) {
	cerr << "Move" << endl;
}

void Entity::execute(StopCommand& c) {
	cerr << "Stop" << endl;
}

void Entity::execute(RepairCommand& c) {
	cerr << "Repair" << endl;
}

void Entity::execute(GatherCommand& c) {
	cerr << "Gather" << endl;
}

void Entity::execute(AttackCommand& c) {
	cerr << "Attack" << endl;
}

void Entity::execute(BuildCommand& c) {
	cerr << "Build" << endl;
}

void Entity::execute(CreateCommand& c) {
	cerr << "Create" << endl;
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

void Unit::addTarget(r2 newTarget) {
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
	bool closed[100][100]; //TODO: Fix
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

void Unit::unsetTarget() {
	waypoints.clear();
}

r2 Unit::target() {
	return waypoints.size() > 0 ?
		waypoints.front() :
		r2(0, 0);
}

bool Unit::targeted() {
	return waypoints.size() > 0;
}

r2 Unit::trajectory() {
	return target() - center();
}

double Unit::sqDistance() {
	return trajectory().sqLength();
}

double Unit::distance() {
	return trajectory().length();
}

Directions Unit::getDirection() {
	return static_cast<Directions>((unsigned)floor(4 * orientation / M_PI + .5) % 8);
}

Unit::~Unit() {}

Unit::Unit(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hitforce, unsigned int hitradius) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	HealthMixin(health),
	speed(speed),
	hit_force(hitforce),
	hit_radius(hitradius)
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
		if (!targeted()) {
			clearCommand(); // At target? TODO: neater
		}
	}
}

void Unit::visit(EntityVisitor& e) {
	e.visit(*this);
}

// TODO: Rest of commands
void Unit::execute(MoveCommand& c) {
	// TODO: on first run of this command, clear waypoints
	if (!targeted()) {
		if (canEnter(c.position)) {
			cerr << "Adding target" << endl;
			addTarget(c.position);
		} else {
			clearCommand();
		}
	}
}

void Unit::execute(StopCommand& c) {
	unsetTarget();
	clearCommand();
}


Worker::Worker(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius, std::vector<Budget> workerProducts) :
	Unit(name, board, owner, position, size, speed, sight_radius, solid, health, hit_force, hit_radius),
	products(workerProducts)
{}

void Worker::update() {
	Unit::update();
}

void Worker::visit(EntityVisitor& e) {
	e.visit(*this);
}


King::King(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius) :
	Unit(name, board, owner, position, size, speed, sight_radius, solid, health, hit_force, hit_radius)
{}

void King::update() {
	Unit::update();
}

void King::visit(EntityVisitor& e) {
	e.visit(*this);
}


Building::Building(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health, std::vector<Budget> producerProducts = {}) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	HealthMixin(health),
	products(producerProducts)
{}

void Building::update() {
	Entity::update();
}

Building::~Building() {
}

void Building::visit(EntityVisitor& e) {
	e.visit(*this);
}


UnfinishedBuilding::UnfinishedBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health) :
	Building(name, board, owner, position, size, sight_radius, solid, health)
{}

void UnfinishedBuilding::update() {
	Building::update();
}

void UnfinishedBuilding::visit(EntityVisitor& e) {
	e.visit(*this);
}


Flag::Flag(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	HealthMixin(health)
{}

void Flag::update() {
	Entity::update();
}

void Flag::visit(EntityVisitor& e) {
	e.visit(*this);
}


TownCenter::TownCenter(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health, std::vector<Budget> producerProducts) :
	Building(name, board, owner, position, size, sight_radius, solid, health, producerProducts)
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


Resource::Resource(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int capacity, std::string resourceName) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	CargoMixin(capacity),
	resource_name(resourceName)
{}

void Resource::update() {
	Entity::update();
}

void Resource::collide(Entity& other) {
	if (!getDeletable() &&
		!other.getDeletable()) {
		stringstream message;
		message << "Un " << other.name << " de " << other.owner.name << " encontró" << resource_name;
		if (other.owner.grantResources(resource_name, cargo.get())) {
			setDeletable();
			message << "; ahora " << other.owner.name
				<< " tiene " << other.owner.getResources()[resource_name]
				<< " " << resource_name;
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

void EntityVisitor::visit(TownCenter& t) {
	visit((Building&)t);
}

void EntityVisitor::visit(Flag& f) {
	visit((Entity&)f);
}

void EntityVisitor::visit(Resource& r) {
	visit((Entity&)r);
}

void EntityVisitor::visit(Terrain& r) {
	visit((Entity&)r);
}

//-------------------------------------------------------------------- PARAMETRO SELECCIONADO
std::shared_ptr<Command> Entity::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Unit::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Worker::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> King::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Building::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> TownCenter::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> UnfinishedBuilding::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Terrain::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Resource::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Flag::defaultCommand(Entity& u) {
	if (!deletable &&
		!u.getDeletable()) {
		return u.giveDefaultCommand(*this);
	}
	return nullptr;
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Entity& u) {
	return nullptr;
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Unit& u) {
	return giveDefaultCommand((Entity&)u);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Worker& w) {
	return giveDefaultCommand((Unit&)w);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(King& k) {
	return giveDefaultCommand((Unit&)k);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Building& b) {
	return giveDefaultCommand((Entity&)b);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(UnfinishedBuilding& u) {
	return giveDefaultCommand((Building&)u);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(TownCenter& t) {
	return giveDefaultCommand((Building&)t);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Flag& f) {
	return giveDefaultCommand((Entity&)f);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Resource& r) {
	return giveDefaultCommand((Entity&)r);
}

std::shared_ptr<Command> Entity::giveDefaultCommand(Terrain& r) {
	return giveDefaultCommand((Entity&)r);
}

std::shared_ptr<Command> Unit::giveDefaultCommand(Entity& r) {
	return nullptr;
}

std::shared_ptr<Command> Unit::giveDefaultCommand(Unit& r) {
	if (owner.getActive() && (owner.name != r.owner.name))
		return std::make_shared<AttackCommand>(r.getId(), getId());
	else
		return giveDefaultCommand((Entity&)r);
}

std::shared_ptr<Command> Building::giveDefaultCommand(Entity& r) {
	return nullptr;
}

std::shared_ptr<Command> Building::giveDefaultCommand(Unit& r) {
	if (owner.getActive() && (owner.name != r.owner.name))
		return std::make_shared<AttackCommand>(r.getId(), getId());
	else
		return giveDefaultCommand((Entity&)r);
}

std::shared_ptr<Command> Building::giveDefaultCommand(Worker& r) {
	if (owner.getActive() && (owner.name != r.owner.name))
		return giveDefaultCommand((Unit&)r);
	else
		return std::make_shared<RepairCommand>(r.getId(), getId());
}

std::shared_ptr<Command> Flag::giveDefaultCommand(Entity& r) {
	return nullptr;
}

std::shared_ptr<Command> Flag::giveDefaultCommand(Unit& r) {
	if (owner.getActive() && (owner.name != r.owner.name))
		return std::make_shared<AttackCommand>(r.getId(), getId());
	else
		return giveDefaultCommand((Entity&)r);
}

std::shared_ptr<Command> Resource::giveDefaultCommand(Worker& r) {
	return std::make_shared<GatherCommand>(r.getId(), getId());
}



