//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
#include <deque>

#include "../defines/defines.h"
#include "geometry.h"
#include "player.h"

//-----------------------------------------------------------------------------

class Board;

class Entity {
	protected:
		r2 position;	// Position (tile)
		double speed;	// Speed (tiles/s)
		std::deque<r2> waypoints;
		bool deletable;
		size_t id;
		bool solid;
		double orientation;
		bool adjustPosition();
		Entity();
		void collide(Entity& other);
		bool canEnter(r2 newPosition);
	public:
		Player& owner;
		r2 size;
		void addTarget(r2 newTarget);
		void unsetTarget();
		r2 target();
		bool targeted();
		const std::string name;
		Board& board;
		r2 center();
		int sight_radius;
		r2 getPosition();
		double getX();// TODO: Deprecar
		double getY();// TODO: Deprecar
		r2 trajectory();
		double sqDistance();
		double distance();
		Directions getDirection();// TODO: Pertenece a vista
		void setDeletable();
		bool getDeletable();
		size_t getId();

		template<typename L> void mapVisible(L fun);

		Entity(std::string name, Board& board, Player& owner, r2 position, r2 size, double speed, int sight_radius, bool solid);
		~Entity();

		void update();

		bool operator==(Entity& other);
		bool operator!=(Entity& other);
};

#include "entity.cxx"
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
