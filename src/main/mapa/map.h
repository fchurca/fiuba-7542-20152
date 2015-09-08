#ifndef SRC_MAIN_MAP_MAP_H_
#define SRC_MAIN_MAP_MAP_H_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include "../entidades/entity.h"
#include "../gfx/sprite_sheet.h"
//-----------------------------------------------------------------------------
class Map {

private:
	std::vector<Entity*> entities;
	std::vector<SpriteSheet*> spriteSheets;

private:
	Map();
public:
	static Map* map;
	static Map* getInstance();
	virtual ~Map();

public:
	void buildMap(ParserYAML* parser);
};
//-----------------------------------------------------------------------------
#endif /* SRC_MAIN_MAP_MAP_H_ */
//-----------------------------------------------------------------------------
