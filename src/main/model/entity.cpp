#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "entity.h"
#include "board.h"
#include "entity_factory.h"

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


HealthMixin::HealthMixin(int max, int armour) :
	health(max), armour(armour)
{}

HealthMixin::HealthMixin(int max, int value, int armour) :
	health(max, value), armour(armour)
{}

HealthMixin::HealthMixin(int min, int max, int value, int armour) :
	health(min, max, value), armour(armour)
{}

ProgressMixin::ProgressMixin(int max) :
	progress(max)
{}

ProgressMixin::ProgressMixin(int max, int value) :
	progress(max, value)
{}

ProgressMixin::ProgressMixin(int min, int max, int value) :
	progress(min, max, value)
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
	executing = false;
	isInAction = false;
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

void Entity::conquered(Player& p) {
	setDeletable();
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
	executing = false;
	isInAction = false;
	command = newCommand;
}

void Entity::update() {
	if (command) {
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

	cerr << end.x << '\t' << end.y << " => " << start.x << '\t' << start.y << '\t' << endl;
	auto targetEntity = board.findEntity(start);
	rectangle targetFootprint;
	cerr << targetEntity;
	if (targetEntity) {
		cerr << '\t' << targetEntity->getPosition().x << '\t' << targetEntity->getPosition().y;
		targetFootprint = rectangle(targetEntity->getPosition(), targetEntity->size);
	}
	cerr << endl;
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
				if (closed[(int)floor(p.x)][(int)floor(p.y)]) {
					continue;
				}
				if ((owner.getVisibility(p) != INVISIBLE)) {
					auto nextFootprint = rectangle::box(p - size / 2, cpos - size / 2, size);
					if (!targetFootprint.intersects(nextFootprint)) {
						if (!(canEnter(nextFootprint))) {
							cerr << p.x << '\t' << p.y << '\t' << "XX" << endl;
							continue;
						}
					}
				}
				auto n = make_shared<TSNode>(p, (cpos - p).length() + c->g, .0, c);
				n->f = f(*n);
				cerr << p.x << '\t' << p.y << '\t' << "OK" << endl;
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

Unit::Unit(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid,unsigned int health, unsigned int armour, unsigned int hitforce, unsigned int hitradius) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	HealthMixin(health, armour),
	speed(speed),
	hitForce(hitforce),
	hitRadius(hitradius)
{
	entityTarget = nullptr;
}

void Unit::update() {
	Entity::update();
}

void Unit::step() {
	if (targeted()) {
		auto traj = trajectory();
		orientation = atan2(traj.y, traj.x);
		auto dr = speed*board.dt / 1000;
		if (pow(dr, 2) < sqDistance()) {
			auto newPos = position + r2::fromPolar(orientation, dr);
			if (!canEnter(newPos)) {
				auto newTarget = waypoints.back();
				unsetTarget();
				addTarget(newTarget);
				return;
			}
			position = newPos;
		} else {
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

void Unit::conquered(Player& p) {
	GameModes modeGame = NOTHING;
	if (modeGame == DESTROY_FLAG && !getDeletable()) {
		Entity::conquered(p);
		owner.board.createEntity(name, p.name, getPosition());
	}
	Entity::conquered(p);
}

// TODO: Rest of commands
void Unit::execute(MoveCommand& c) {
	if (!executing) {
		unsetTarget();
		executing = true;
		addTarget(c.position);
	}
	step();
	if (!targeted()) {
		clearCommand();
	}
}

void Unit::execute(StopCommand& c) {
	unsetTarget();
	clearCommand();
}

void Unit::execute(AttackCommand& c) {
	isInAction = false;
	if (!executing) {
		entityTarget = owner.board.findEntity(c.targetId);
		if (entityTarget) {
			if (!entityTarget->getDeletable()) {
				executing = true;
				addTarget(entityTarget->center());
				return;
			}
		}
		entityTarget = nullptr;
		clearCommand();
		return;
	}
	else {
		if (!getDeletable() && !entityTarget->getDeletable() && owner.getVisibility(*entityTarget) > INVISIBLE) {
			rectangle ataque(getPosition() - r2(hitRadius, hitRadius), r2(2 * hitRadius + size.x, 2 * hitRadius + size.y));//Vision en forma de Cuadrado
			if (ataque.intersects(rectangle(entityTarget->getPosition(), entityTarget->size))) {
				auto entity = dynamic_cast<HealthMixin*>(entityTarget.get());
				if (entity) {
					if (!(entity->armour > hitForce)) {
						entity->health.inc(-1 * hitForce);
					}
					if (entity->health.get() == entity->health.min) {
						entityTarget->setDeletable();
						if (!entityTarget->owner.getAlive()) {
							owner.conquer(entityTarget->owner);
							return;
						}
					}
					else {
						isInAction = true;
						return;
					}
				}
			}
			else {
				if (!targeted()) {
					addTarget(entityTarget->center());
				}
				step();
				return;
			}
		}
		entityTarget = nullptr;
		clearCommand();
		return;
	}
}


Worker::Worker(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid,unsigned int health, unsigned int armour, unsigned int hit_force, unsigned int hit_radius, std::vector<Budget> workerProducts) :
	Unit(name, board, owner, position, size, speed, sight_radius, solid, health, armour,hit_force, hit_radius),
	products(workerProducts)
{}

void Worker::update() {
	Unit::update();
}

void Worker::visit(EntityVisitor& e) {
	e.visit(*this);
}

void Worker::execute(GatherCommand& c) {
	isInAction = false;
	if (!executing) {
		entityTarget = owner.board.findEntity(c.targetId);
		if (!entityTarget) {
			clearCommand();
			return;
		}
		executing = true;
		addTarget(entityTarget->center());
	}
	else {
		if (!entityTarget->getDeletable() && !getDeletable()) {
			auto resource = dynamic_cast<Resource*>(entityTarget.get());
			if (resource) {
				rectangle ataque(getPosition() - r2(hitRadius, hitRadius),r2(2*hitRadius + size.x, 2*hitRadius + size.y));//Vision en forma de Cuadrado
				if (ataque.intersects(rectangle(resource->getPosition(), resource->size))){
					if (resource->cargo.get() == resource->cargo.min) {
						resource->setDeletable();
						entityTarget = nullptr;
						//Buscar siguiente recurso a recolectar
						//std::vector<std::shared_ptr<Entity>> resources = owner.board.selectEntities(rectangle(getPosition() - r2(sight_radius,sight_radius), r2(2*sight_radius,2*sight_radius)));
						//for (auto& r : resources) {
						//	auto nextResource = dynamic_cast<Resource*>(r.get());
						//	if (nextResource) {
						//		if (nextResource->resource_name == resource->resource_name) {
						//			setCommand(std::make_shared<GatherCommand>(getId(), nextResource->getId()));
						//			return;
						//		}
						//	}
						//}
					}
					else {
						isInAction = true;
						owner.grantResources(resource->resource_name, 1);
						resource->cargo.inc(-1);
						return;
					}
				}
				else {
					step();
					return;
				}
			}
		}
		entityTarget = nullptr;
		clearCommand();
		return;
	}
}
void Worker::execute(RepairCommand& c) {
	isInAction = false;
	if (!executing) {
		entityTarget = owner.board.findEntity(c.targetId);
		if (!entityTarget) {
			clearCommand();
		}
		executing = true;
		addTarget(entityTarget->center());
	}
	else {
		if (!entityTarget->getDeletable() && !getDeletable()) {
			rectangle ataque(getPosition() - r2(hitRadius, hitRadius), r2(2 * hitRadius + size.x, 2 * hitRadius + size.y));//Vision en forma de Cuadrado
			if (ataque.intersects(rectangle(entityTarget->getPosition(), entityTarget->size))) {
				auto unfinichedBuilding = dynamic_cast<UnfinishedBuilding*>(entityTarget.get());
				if (unfinichedBuilding) {
					if (unfinichedBuilding->progress.get() < unfinichedBuilding->progress.max) {
						unfinichedBuilding->progress.inc(1);
						isInAction = true;
						return;
					}
					if (unfinichedBuilding->progress.get() == unfinichedBuilding->progress.max) {
						entityTarget->setDeletable();
						owner.board.createEntity(entityTarget->name, entityTarget->owner.name, entityTarget->getPosition());
						isInAction = false;
					}
				}
				else {
					auto building = dynamic_cast<Building*>(entityTarget.get());
					if (building) {
						if (building->health.get() < building->health.max) {
							building->health.inc(1);
							isInAction = true;
							return;
						}
					}
				}
			}
			else {
				step();
				return;
			}
		}
		entityTarget = nullptr;
		clearCommand();
		return;
	}
}

void Worker::execute(BuildCommand& c) {
	if (!executing) { // Primera vez valido los recursos
		int i = 0;
		for (auto& p : products) {
			if (p.name == c.entityType) {
				break;
			}
			i++;
		}
		bool haveResources = true;
		for (auto& c : products[i].lines) {
			if (c.amount > owner.getResources()[c.resource_name]) {
				haveResources = false;
				break;
			}
		}
		if (!haveResources) {
			clearCommand();
			return;
		}
		executing = true;
	}
	else {
		if (!getDeletable()) {
			std::shared_ptr<EntityFactory> entityFactory = owner.board.entityFactories[c.entityType];
			auto entityFactoryUnfinished = dynamic_cast<BuildingFactory*>(entityFactory.get());
			if (entityFactoryUnfinished) {
				std::shared_ptr<Entity> entity = entityFactoryUnfinished->createUnfinished(owner, c.position);
				if (owner.board.createEntity(entity)) {
					int i = 0;
					for (auto& p : products) {
						if (p.name == c.entityType) {
							break;
						}
						i++;
					}
					for (auto& c : products[i].lines) {
						owner.grantResources(c.resource_name, -1 * c.amount);
					}
					setCommand(std::make_shared<RepairCommand>(getId(), entity->getId()));
					return;
				}
			}
		}
		clearCommand();
		return;
	}
}


King::King(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, unsigned int health, unsigned int armour, unsigned int hit_force, unsigned int hit_radius) :
	Unit(name, board, owner, position, size, speed, sight_radius, solid, health, armour, hit_force, hit_radius)
{}

void King::update() {
	Unit::update();
}

void King::visit(EntityVisitor& e) {
	e.visit(*this);
}

void King::conquered(Player& p){
	Entity::conquered(p);
}


Building::Building(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid,unsigned int health, unsigned int armour, std::vector<Budget> producerProducts = {}) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	HealthMixin(health, armour),
	ProgressMixin(0,100,0),
	products(producerProducts)
{
	currentProduct = "";
}

void Building::update() {
	Entity::update();
}

void Building::conquered(Player& p) {
	GameModes modeGame = NOTHING;
	if (!getDeletable()) {
		Entity::conquered(p);
		if (modeGame == DESTROY_CENTER || modeGame == KILL_KING) {
			owner.board.createEntity(name, DEFAULT_PLAYER_NAME, getPosition());
		}
		else {
			owner.board.createEntity(name, p.name, getPosition());
		}
	}
	Entity::conquered(p);
}

void Building::execute(CreateCommand& c) {
	isInAction = false;
	if (!executing) { // Primera vez valido los recursos
		progress.set(0);
		int i = 0;
		for (auto& p : products) {
			if (p.name == c.entityType) {
				break;
			}
			i++;
		}
		bool haveResources = true;
		for (auto& c : products[i].lines) {
			if (c.amount > owner.getResources()[c.resource_name]) {
				haveResources = false;
				break;
			}
		}
		if (!haveResources) {
			clearCommand();
			return;
		}
		for (auto& c : products[i].lines) {
			owner.grantResources(c.resource_name , -1 * c.amount);
		}
		executing = true;
	}
	else {
		if (!getDeletable()) {
			if (progress.get() < progress.max) {
				progress.inc(1);
				isInAction = true;
				return;
			}
			if (progress.get() == progress.max) {
				//TODO VER LA POSICION DONDE SE CREA
				int i = getPosition().x + 1;
				int j = getPosition().y + 1;
				bool imposibleSet = false;
				while (!owner.board.createEntity(c.entityType, owner.name, r2(i, j)) || imposibleSet) {
					if (j < owner.board.sizeY) {
						j++;
					}
					else {
						imposibleSet = true;
					}		
				}
				isInAction = false;
			}
		}
		clearCommand();
		return;
	}
}

Building::~Building() {
}

void Building::visit(EntityVisitor& e) {
	e.visit(*this);
}


UnfinishedBuilding::UnfinishedBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid,unsigned int health, unsigned int armour) :
	Building(name, board, owner, position, size, sight_radius, solid, health, armour)
{}

void UnfinishedBuilding::update() {
	Building::update();
}

void UnfinishedBuilding::visit(EntityVisitor& e) {
	e.visit(*this);
}


Flag::Flag(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid,unsigned int health, unsigned int armour) :
	Entity(name, board, owner, position, size, sight_radius, solid),
	HealthMixin(health, armour)
{}

void Flag::update() {
	Entity::update();
}

void Flag::visit(EntityVisitor& e) {
	e.visit(*this);
}


TownCenter::TownCenter(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid,unsigned int health, unsigned int armour, std::vector<Budget> producerProducts) :
	Building(name, board, owner, position, size, sight_radius, solid, health, armour, producerProducts)
{}

void TownCenter::update() {
	Building::update();
}

void TownCenter::visit(EntityVisitor& e) {
	e.visit(*this);
}

void TownCenter::conquered(Player& p) {
	Entity::conquered(p);
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
	if (owner.name != r.owner.name)//TODO VER SI ESTA ACTIVO?
		return std::make_shared<AttackCommand>(r.getId(), getId());
	else
		return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Unit::giveDefaultCommand(King& r) {
		return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Building::giveDefaultCommand(Unit& r) {
	if (owner.name != r.owner.name)//TODO VER SI ESTA ACTIVO?
		return std::make_shared<AttackCommand>(r.getId(), getId());
	else
		return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Building::giveDefaultCommand(King& r) {
		return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Building::giveDefaultCommand(Worker& r) {
	if (owner.name != r.owner.name)
		return giveDefaultCommand((Unit&)r);
	else
		return std::make_shared<RepairCommand>(r.getId(), getId());
}

std::shared_ptr<Command> Flag::giveDefaultCommand(Unit& r) {
	if (owner.name != r.owner.name) //TODO VER SI ESTA ACTIVO?
		return std::make_shared<AttackCommand>(r.getId(), getId());
	else
		return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Flag::giveDefaultCommand(King& r) {
		return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Resource::giveDefaultCommand(Unit& r) {
	return std::make_shared<MoveCommand>(r.getId(), center());
}

std::shared_ptr<Command> Resource::giveDefaultCommand(Worker& r) {
	return std::make_shared<GatherCommand>(r.getId(), getId());
}



