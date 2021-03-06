#ifndef __GFX_MINIMAP_H__
#define __GFX_MINIMAP_H__
#include <string>
#include <SDL2/SDL.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class MiniMap{
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
	r2 scale;
	SDL_Color tmpGetColor(std::string name);
protected:
public:
	void draw();
	SDL_Point boardToScreenPosition(r2 boardPos);
	MiniMap(GameWindow& owner, GraphicsParser& graphicsParser);
	~MiniMap();
};
#endif // __GFX_MINIMAP_H__
