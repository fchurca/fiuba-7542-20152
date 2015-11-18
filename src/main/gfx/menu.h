#ifndef __GFX_MENU_H__
#define __GFX_MENU_H__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../model/geometry.h"
class GameWindow;
class GraphicsParser;

class Menu {
private:
	GameWindow& owner;
	r2 size;
	r2 offset;
protected:
public:
	void draw(SDL_Renderer* renderer);
	Menu(GameWindow& owner, GraphicsParser& graphicsParser);
	~Menu();
};
#endif // __GFX_MENU_H__