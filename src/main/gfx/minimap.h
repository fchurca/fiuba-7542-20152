#ifndef __GFX_MINIMAP_H__
#define __GFX_MINIMAP_H__
#include <string>
#include <SDL2/SDL.h>
#include "../model/geometry.h"
class GameWindow;

class MiniMap{
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
	r2 scale;
	SDL_Color getColor(int id);
	SDL_Color tmpGetColor(std::string name);
protected:
public:
	void drawMinimap(SDL_Renderer* renderer);
	MiniMap(GameWindow& owner);
	~MiniMap();
};
#endif // __GFX_MINIMAP_H__
