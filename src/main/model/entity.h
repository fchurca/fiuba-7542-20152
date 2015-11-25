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

//-----------------------------------------------------------------------------

class ABoard;
class Resource;
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
		unsigned int hit_force;
		unsigned int hit_radius;
		r2 trajectory();
		virtual void addTarget(r2 newTarget);
		virtual void unsetTarget();
		double sqDistance();
		double distance();
		Directions getDirection();// TODO: Pertenece a vista
		r2 target();
		bool targeted();
		Unit(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius);
		virtual void update();
		virtual void visit(EntityVisitor& v);
		bool getIsInAction();
		virtual ~Unit();
		// TODO: Rest of commands
		virtual void execute(MoveCommand& c);
		virtual void execute(StopCommand& c);
};

class Worker : public Unit {
	public:
		void update();
		Worker(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius, std::map<std::string, std::map<std::string, unsigned int>> workerProducts);
		virtual void visit(EntityVisitor& v);
		std::map<std::string, std::map<std::string, unsigned int>> products;
};

class King : public Unit {
	public:
		void update();
		King(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid, int health, unsigned int hit_force, unsigned int hit_radius);
		virtual void visit(EntityVisitor& v);
};

class Building : public Entity, public HealthMixin {
	public:
		virtual void update();
		Building(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health, std::map<std::string, std::map<std::string, unsigned int>> producerProducts);
		std::map<std::string, std::map<std::string, unsigned int>> products;
		virtual void visit(EntityVisitor& v);
		virtual ~Building();
};

class UnfinishedBuilding : public Building {
	public:
		void update();
		UnfinishedBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health);
		virtual void visit(EntityVisitor& v);
};

class TownCenter : public Building {
	public:
		void update();
		TownCenter(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health, std::map<std::string, std::map<std::string, unsigned int>> producerProducts);
		virtual void visit(EntityVisitor& v);
};

class Flag : public Entity, public HealthMixin {
	public:
		void update();
		Flag(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int health);
		virtual void visit(EntityVisitor& v);
};

class Resource : public Entity , public CargoMixin {
	protected:
		void update();
		std::string resource_name;
		void collide(Entity& other);
		void collide(Resource& other);
	public:
		Resource(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int capacity, std::string resourceName);
		virtual void visit(EntityVisitor& v);
};

class Terrain : public Entity {
protected:
	void update();
public:
	Terrain(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
	virtual void visit(EntityVisitor& v);
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
