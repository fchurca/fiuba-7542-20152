#ifndef _SRC_MAIN_BOARD_BOARD_H_
#define _SRC_MAIN_BOARD_BOARD_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include "../entidades/entity.h"
#include "../gfx/sprite_sheet.h"
#include "../parser_yaml/parser_yaml.h"
//-----------------------------------------------------------------------------
class Board {

private:
	std::vector<Entity*> entities;
	std::vector<SpriteSheet*> spriteSheets;

public:
	~Board(){}

	void buildBoard(ParserYAML* parser);
};
//-----------------------------------------------------------------------------
#endif /* _SRC_MAIN_BOARD_BOARD_H_ */
//-----------------------------------------------------------------------------
