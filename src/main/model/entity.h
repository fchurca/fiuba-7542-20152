//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
#include <deque>

#include "../defines/defines.h"
#include "geometry.h"
#include "mixins.h"
#include "player.h"

//-----------------------------------------------------------------------------

class ABoard;
class Resource;

class Entity : public IdMixin, public FrameMixin, public DeletableMixin {
	protected:
		r2 position;	// Position (tile)
		std::deque<r2> waypoints;
		bool solid;
		double orientation;
		bool adjustPosition();
		virtual void collide(Entity* other);
		virtual void collide(Entity& other);
		virtual void collide(Resource& other);
		bool canEnter(rectangle r);
		bool canEnter(r2 newPosition);
		void setFrame();
	public:
		Player& owner;
		r2 size;
		virtual void addTarget(r2 newTarget);
		virtual void unsetTarget();
		r2 target();
		bool targeted();
		const std::string name;
		ABoard& board;
		r2 center();
		int sight_radius;
		r2 getPosition();
		void setPosition(r2 newPos);
		r2 trajectory();
		double sqDistance();
		double distance();
		double getOrientation();
		void setOrientation(double newOrientation);
		Directions getDirection();// TODO: Pertenece a vista

		template<typename L> void mapVisible(L fun);

		Entity(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
		virtual ~Entity();

		virtual void update();

		virtual void setDeletable();
		void setFrame(size_t newFrame);

		bool operator==(Entity& other);
		bool operator!=(Entity& other);
};

class Unit : public Entity {
	protected:
		double speed;	// Speed (tiles/s)
	public:
		Unit(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid);
		virtual void update();
		virtual ~Unit();
};

class Worker : public Unit {
	public:
		void update();
		Worker(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid);
};

class King : public Unit {
	public:
		void update();
		King(std::string name, ABoard& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid);
};

class Building : public Entity {
	public:
		virtual void update();
		Building(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
};

class UnfinishedBuilding : public Building {
	public:
		void update();
		UnfinishedBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
};

class ProducerBuilding : public Building {
	public:
		virtual void update();
		ProducerBuilding(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
};

class TownCenter : public ProducerBuilding {
	public:
		void update();
		TownCenter(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
};

class Flag : public Building {
	public:
		void update();
		Flag(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid);
};

class Resource : public Entity {
	protected:
		void update();
		void collide(Entity& other);
		void collide(Resource& other);
	public:
		Resource(std::string name, ABoard& board, Player& owner, r2 position, r2 size, int sight_radius, bool solid, int capacity);
		int capacity;
};

#include "entity.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
