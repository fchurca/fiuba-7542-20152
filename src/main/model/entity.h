//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
#include "../defines/defines.h"
#include "geometry.h"

//-----------------------------------------------------------------------------

class Board;

class Entity {
	protected:
		r2 position;	// Position (tile)
		double speed;	// Speed (tiles/s)
		bool targeted;
		r2 target;

		Entity();
		bool adjustPosition();

	public:
		r2 size;
		void setTarget(double x, double y);
		void unsetTarget();
		const std::string name;
		const Board& board;
		r2 center();
		r2 getPosition();
		double getX();
		double getY();
		r2 trajectory();
		double bearing();
		double sqDistance();
		double distance();
		Directions getDirection();
		bool overlaps(Entity & other);

		Entity(std::string name, Board& board, double x, double y, double sizeX, double sizeY, double speed);
		~Entity();

		void update();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
