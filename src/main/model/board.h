#ifndef _MODEL_BOARD_H_
#define _MODEL_BOARD_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include "../model/entity.h"
#include "../parser_yaml/parser_yaml.h"
//-----------------------------------------------------------------------------
class Board {

private:
	std::vector<Entity*> entities;
	Board();

public:
	const int
		sizeX, sizeY;
	Board(int sizeX, int sizeY);
	~Board();

	void createEntity(std::string name, double x, double y);
	void update();
	void buildBoard(ParserYAML* parser);
};
//-----------------------------------------------------------------------------
#endif /* _MODEL_BOARD_H_ */
//-----------------------------------------------------------------------------
