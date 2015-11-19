#ifndef __GFX_PLAYERS_LIST_H__
#define __GFX_PLAYERS_LIST_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class PlayersList {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
protected:
public:
	void draw();
	PlayersList(GameWindow& owner, GraphicsParser& graphicsParser);
	~PlayersList();
};
#endif // __GFX_PLAYERS_LIST_H__