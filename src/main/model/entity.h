//-----------------------------------------------------------------------------
#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

class Board;

class Entity {
	protected:
		double x, y;	// Position (tile)
		double speed;	// Speed (tiles/s)

		Entity();
		void adjustPosition();

	public:
		const std::string name;
		const Board& board;

		Entity(std::string name, Board& board, double x, double y);
		~Entity();

		void update();
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_ENTITY_H_ */
//-----------------------------------------------------------------------------
