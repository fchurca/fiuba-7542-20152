//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
#include <deque>

#include "../defines/defines.h"
#include "geometry.h"

//-----------------------------------------------------------------------------

class Board;

class Entity {
	protected:
		r2 position;	// Position (tile)
		double speed;	// Speed (tiles/s)
		std::deque<r2> targets;

		Entity();
		bool adjustPosition();

	public:
		r2 size;
		void addTarget(r2 newTarget);
		void unsetTarget();
		r2 target();
		bool targeted();
		const std::string name;
		const Board& board;
		r2 center();
		r2 getPosition();
		double getX();// TODO: Deprecar
		double getY();// TODO: Deprecar
		r2 trajectory();
		double bearing();
		double sqDistance();
		double distance();
		Directions getDirection();// TODO: Pertenece a vista
		bool overlaps(Entity & other);

		Entity(std::string name, Board& board, r2 position, r2 size, double speed);
		~Entity();

		void update();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
