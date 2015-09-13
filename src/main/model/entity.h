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
		void adjustPosition();

	public:
		void setTarget(double x, double y);
		void unsetTarget();
		const std::string name;
		const Board& board;
		int getX();
		int getY();
		Directions getDirection();

		Entity(std::string name, Board& board, double x, double y);
		~Entity();

		void update();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
