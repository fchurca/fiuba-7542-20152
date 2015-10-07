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
		std::deque<r2> targets;
		bool deletable;
		size_t id;
		bool adjustPosition();
		Entity();
		void collide(Entity& other);
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
		double bearing();
		double sqDistance();
		double distance();
		Directions getDirection();// TODO: Pertenece a vista
		void setDeletable();
		bool getDeletable();
		size_t getId();

		Entity(std::string name, Board& board, Player& owner, r2 position, r2 size, double speed, int sight_radius);
		~Entity();

		void update();

		bool operator==(Entity& other);
		bool operator!=(Entity& other);
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
