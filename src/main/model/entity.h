//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <deque>
#include <string>

#include "../defines/defines.h"
#include "command.h"
#include "geometry.h"
#include "mixins.h"
#include "player.h"
#include "budget.h"

//-----------------------------------------------------------------------------

class ABoard;
class Entity;
class Unit;
class Worker;
class King;
class Flag;
class Building;
class TownCenter;
class UnfinishedBuilding;
class Resource;
class Terrain;
class EntityVisitor;


class EntityMixin {};

class CargoMixin : public EntityMixin {
	public:
		Gauge cargo;
		CargoMixin(int max);
		CargoMixin(int min, int max);
		CargoMixin(int min, int max, int value);
};


class HealthMixin : public EntityMixin {
	public:
		Gauge health;
		HealthMixin(int max);
		HealthMixin(int min, int max);
		HealthMixin(int min, int max, int value);
};


class Entity : public IdMixin, public FrameMixin, public DeletableMixin {
	protected:
		r2 position;	// Position (tile)
		bool solid;
		bool adjustPosition();
		virtual void collide(Entity* other);
		virtual void collide(Entity& other);
		virtual void collide(Resource& other);
		bool canEnter(rectangle r);
		bool canEnter(r2 newPosition);
		void setFrame();
		double orientation;
	public:
		Player& owner;
		r2 size;
		const std::string name;
		ABoard& board;
		r2 center();
		int sight_radius;
		r2 getPosition();
		void setPosition(r2 newPos);
		double getOrientation();
		void setOrientation(double newOrientation);

		template<typename L> void mapVisible(L fun);

		Entity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
		virtual ~Entity();

		virtual void update();

		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		virtual std::shared_ptr<Command> giveDefaultCommand(Entity& e);
		virtual std::shared_ptr<Command> giveDefaultCommand(Unit& u);
		virtual std::shared_ptr<Command> giveDefaultCommand(Worker& w);
		virtual std::shared_ptr<Command> giveDefaultCommand(King& k);
		virtual std::shared_ptr<Command> giveDefaultCommand(Building& b);
		virtual std::shared_ptr<Command> giveDefaultCommand(UnfinishedBuilding& u);
		virtual std::shared_ptr<Command> giveDefaultCommand(TownCenter& t);
		virtual std::shared_ptr<Command> giveDefaultCommand(Flag& f);
		virtual std::shared_ptr<Command> giveDefaultCommand(Resource& r);
		virtual std::shared_ptr<Command> giveDefaultCommand(Terrain& t);

		virtual void setDeletable();
		void setFrame(size_t newFrame);

		bool operator==(Entity& other);
		bool operator!=(Entity& other);

		virtual void visit(EntityVisitor& v) = 0;
		// TODO: Rest of commands
		virtual void execute(MoveCommand& c);
		virtual void execute(StopCommand& c);
};

class Unit : public Entity, public HealthMixin {
	protected:
		std::deque<r2> waypoints;
		double speed;	// Speed (tiles/s)
		bool isInAction = false;
	public:
		unsigned int hitForce;
		unsigned int hitRadius;
		r2 trajectory();
		virtual void addTarget(r2 newTarget);
		virtual void unsetTarget();
		double sqDistance();
		double distance();
		Directions getDirection();// TODO: Pertenece a vista
		r2 target();
		bool targeted();
		Unit(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hitForce, unsigned int hit_radius);
		virtual void update();
		virtual void visit(EntityVisitor& v);
		bool getIsInAction();
		virtual ~Unit();
		// TODO: Rest of commands
		virtual void execute(MoveCommand& c);
		virtual void execute(StopCommand& c);
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		virtual std::shared_ptr<Command> giveDefaultCommand(Entity& u);
		virtual std::shared_ptr<Command> giveDefaultCommand(Unit& u);
};

class Worker : public Unit {
	public:
		void update();
		Worker(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hitForce, unsigned int hit_radius, std::vector<Budget> workerProducts);
		virtual void visit(EntityVisitor& v);
		std::vector<Budget> products;
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
};

class King : public Unit {
	public:
		void update();
		King(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hitForce, unsigned int hit_radius);
		virtual void visit(EntityVisitor& v);
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
};

class Building : public Entity, public HealthMixin {
	public:
		virtual void update();
		Building(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health, std::vector<Budget> producerProducts);
		std::vector<Budget> products;
		virtual void visit(EntityVisitor& v);
		virtual ~Building();
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		virtual std::shared_ptr<Command> giveDefaultCommand(Entity& u);
		virtual std::shared_ptr<Command> giveDefaultCommand(Unit& u);
		virtual std::shared_ptr<Command> giveDefaultCommand(Worker& u);
};

class UnfinishedBuilding : public Building {
	public:
		void update();
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		UnfinishedBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health);
		virtual void visit(EntityVisitor& v);
};

class TownCenter : public Building {
	public:
		void update();
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		TownCenter(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health, std::vector<Budget> producerProducts);
		virtual void visit(EntityVisitor& v);
};

class Flag : public Entity, public HealthMixin {
	public:
		void update();
		Flag(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health);
		virtual void visit(EntityVisitor& v);
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		virtual std::shared_ptr<Command> giveDefaultCommand(Entity& u);
		virtual std::shared_ptr<Command> giveDefaultCommand(Unit& u);
};

class Resource : public Entity , public CargoMixin {
	protected:
		void update();
		void collide(Entity& other);
		void collide(Resource& other);
	public:
		const std::string resource_name;
		Resource(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int capacity, std::string resourceName);
		virtual void visit(EntityVisitor& v);
		virtual std::shared_ptr<Command> defaultCommand(Entity& other);
		virtual std::shared_ptr<Command> giveDefaultCommand(Worker& u);
};

class Terrain : public Entity {
protected:
	void update();
public:
	Terrain(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
	virtual void visit(EntityVisitor& v);
	virtual std::shared_ptr<Command> defaultCommand(Entity& other);
};


class EntityVisitor {
	public:
		virtual void visit(Entity& e) =0;
		virtual void visit(Unit& u);
		virtual void visit(Worker& w);
		virtual void visit(King& k);
		virtual void visit(Building& b);
		virtual void visit(UnfinishedBuilding& u);
		virtual void visit(TownCenter& t);
		virtual void visit(Flag& f);
		virtual void visit(Resource& r);
		virtual void visit(Terrain& t);
};


#include "entity.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
