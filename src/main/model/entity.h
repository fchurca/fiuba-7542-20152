//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
#include "../defines/defines.h"

//-----------------------------------------------------------------------------

class Board;

class Entity {
	protected:
		double x, y;	// Position (tile)
		double speed;	// Speed (tiles/s)
		bool targeted;
		double targetX, targetY;

		Entity();
		bool adjustPosition();

	public:
		const double
			sizeX, sizeY;
		void setTarget(double x, double y);
		void unsetTarget();
		const std::string name;
		const Board& board;
		double getX();
		double getY();
		double bearingX();
		double bearingY();
		double bearing();
		double sqDistance();
		double distance();
		Directions getDirection();

		Entity(std::string name, Board& board, double x, double y, double sizeX, double sizeY);
		~Entity();

		void update();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
